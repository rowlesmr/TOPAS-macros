'see http://topas.dur.ac.uk/topaswiki/doku.php?id=sequential_refinements

macro Sequential_Refinement(numruns, ident) { Sequential_Refinement(numruns, ident, , INP_File, Run_Number) }
macro Sequential_Refinement(numruns, ident, output) { Sequential_Refinement(numruns, ident, output, INP_File, Run_Number) }
macro Sequential_Refinement(numruns, ident, output, INPFILE, RUNNUMBER) 'This one would almost never be called directly
{
    num_runs numruns

    'saves a backup of the original input
    #if Run_Number == 0;
        system_before_save_OUT { copy INPFILE##.inp INPFILE##.backup }
    #endif

    'Save each input file with the run number and some other identifier
    system_before_save_OUT { copy INPFILE##.inp INPFILE##_##RUNNUMBER##_##ident##.inp }

    'copy the output of the nth file and use it as the input for the (n+1)th file
    out_file = Concat(String(INPFILE), ".INP");

    'copies the backup back over the final input file at the end of the run so you can restart from where you started
    #if Run_Number == numruns-1;
        system_after_save_OUT { copy INPFILE##.backup INPFILE##.inp }
    #endif

    'output every refined parameter
    #m_ifarg output "" #m_else
    	'out_prm_vals_on_convergence output   'this doesn't work for multiple files
    #m_endif
}




'this macro allows the use of arbitrary length output
' use it like: Out_Results(file, Get(phase_name)) { O_R(csG) O_R(Get(a)) }
macro Out_Results(outFileName) { Out_Results(outFileName, file, seqno, Get(phase_name)) }
macro Out_Results(outFileName, phasename) { Out_Results(outFileName, file, seqno, phasename) }
macro Out_Results(outFileName, file, seqno) { Out_Results(outFileName, file, seqno, Get(phase_name)) }
macro Out_Results(outFileName, file, seqno, phasename) {
	out outFileName append
		Out_String("\n")
		Out_String(file)
		Out_String("\t")
		Out_String(seqno)
		Out_String("\t")
		load out_record out_fmt out_eqn
		{
			 "%s"  = phasename;
		}
		load out_record out_fmt out_fmt_err out_eqn
}



macro O_R(& value) {
	"\t%10.9f" "\t%10.9f"
	#m_ifarg value #m_code
		= If(Prm_There(value),value,-1);
	#m_else ''it must be a numeral, an eqn, or a Get()
		=value;
	#m_endif
}

macro O_R_cellprms(themacroval) {
	#m_ifarg themacroval "0"
		 "\t%10.9f" "\t%10.9f" = -1.0 ;
		 "\t%10.9f" "\t%10.9f" = -1.0 ;
		 "\t%10.9f" "\t%10.9f" = -1.0 ;
		 "\t%10.9f" "\t%10.9f" = -1.0 ;
		 "\t%10.9f" "\t%10.9f" = -1.0 ;
		 "\t%10.9f" "\t%10.9f" = -1.0 ;
		 "\t%10.9f" "\t%10.9f" = -1.0 ;
		 "\t%10.9f" "\t%10.9f" = -1.0 ;
	#m_else
		 "\t%10.9f" "\t%10.9f" = Get(a);
		 "\t%10.9f" "\t%10.9f" = Get(b);
		 "\t%10.9f" "\t%10.9f" = Get(c);
		 "\t%10.9f" "\t%10.9f" = Get(al);
		 "\t%10.9f" "\t%10.9f" = Get(be);
		 "\t%10.9f" "\t%10.9f" = Get(ga);
		 "\t%10.9f" "\t%10.9f" = Get(cell_volume);
		 "\t%10.9f" "\t%10.9f" = Get(cell_mass);
	#m_endif
}


macro Out_Header(outFileName) {
	out outFileName append
		Out_String("@file\t")
		Out_String("seqno\t")
		Out_String("phase")
}

macro O_H(head) {
	Out_String("\t")	Out_String(head)
	Out_String("\t")	Out_String(head##_err)
}

macro O_H_cellprms {
	Out_String("\t")	Out_String(a)
	Out_String("\t")	Out_String(a##_err)
	Out_String("\t")	Out_String(b)
	Out_String("\t")	Out_String(b##_err)
	Out_String("\t")	Out_String(c)
	Out_String("\t")	Out_String(c##_err)
	Out_String("\t")	Out_String(al)
	Out_String("\t")	Out_String(al##_err)
	Out_String("\t")	Out_String(be)
	Out_String("\t")	Out_String(be##_err)
	Out_String("\t")	Out_String(ga)
	Out_String("\t")	Out_String(ga##_err)
	Out_String("\t")	Out_String(cellVol)
	Out_String("\t")	Out_String(cellVol##_err)
	Out_String("\t")	Out_String(cellMass)
	Out_String("\t")	Out_String(cellMass##_err)
}








macro O_R_normErr(& value) {
	'the value of the prm
	"\t%10.9f"
	#m_ifarg value #m_code
		= If(Prm_There(value),value,-1);
	#m_else ''it must be a numeral, an eqn, or a Get()
		=value;
	#m_endif

	'the error of the prm
	"\t%10.9f"
	#m_ifarg value #m_code
		= If(Prm_There(value),Get_Error(value)/Get(gof),0);
	#m_else ''it must be a numeral, an eqn, or a Get()
		=Get_Error(value)/Get(gof); 'an eqn will break the macro
	#m_endif
}

macro O_R_noErr(& value) {
	'the value of the prm
	"\t%10.9f"
	#m_ifarg value #m_code
		= If(Prm_There(value),value,-1);
	#m_else ''it must be a numeral, an eqn, or a Get()
		=value;
	#m_endif
}


'todo: this may need updating for v7 - update TA7.7 makes
'  the error of a prm with no error =0, not the value of the prm.
macro Get_Error(& val) {
	If(Error(val) == val, 0, Error(val))
}


macro Prm_to_file(prm) {
	out prm##.txt Out(prm)
}

macro Prm_from_file(prm) {
	#include prm##.txt
}


macro randomValue(value, plusminus)
{
	(Rand(value - plusminus, value + plusminus))
}



'Some calculations you may want to do
macro LVol_v(csg, csl)
{
	#m_ifarg csg ""
		IF Prm_There(csl) THEN
			1 / IB_from_CS(,csl)
		ELSE
			-1
		ENDIF
	#m_else #m_ifarg csl ""
		IF Prm_There(csg) THEN
			1 / IB_from_CS(csg,)
		ELSE
			-1
		ENDIF
	#m_else
		IF And(Prm_There(csg), Prm_There(csl)) THEN
			1 / IB_from_CS(csg, csl)
		ELSE IF Prm_There(csg) THEN
			1 / IB_from_CS(csg,)
		ELSE IF Prm_There(csl) THEN
			1 / IB_from_CS(,csl)
		ELSE
			-1
		ENDIF ENDIF ENDIF
	#m_endif
	#m_endif
}


macro e0_v(strg, strl)
{
	#m_ifarg strg ""
		IF Prm_There(strl) THEN
			Voigt_FWHM_GL(0, strl) .25 Pi/360
		ELSE
			-1
		ENDIF
	#m_else #m_ifarg strl ""
		IF Prm_There(strg) THEN
			Voigt_FWHM_GL(strg, 0) .25 Pi/360
		ELSE
			-1
		ENDIF
	#m_else
		IF And(Prm_There(strg), Prm_There(strl)) THEN
			Voigt_FWHM_GL(strg, strl) .25 Pi/360
		ELSE IF Prm_There(strg) THEN
			Voigt_FWHM_GL(strg, 0) .25 Pi/360
		ELSE IF Prm_There(strl) THEN
			Voigt_FWHM_GL(0, strl) .25 Pi/360
		ELSE
			-1
		ENDIF ENDIF ENDIF
	#m_endif
	#m_endif
}


macro Backup_First_INP { Backup_INP_for_Run_0 }
macro Backup_INP_for_Run_0
{
	#if (Run_Number == 0)
      system_before_save_OUT  {
         copy INP_File##.inp INP_File##before.backup
      }
      system_after_save_OUT  {
         copy INP_File##.inp INP_File##after.backup
      }
    #endif
}
