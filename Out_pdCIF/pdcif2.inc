#ifndef CIF_MACROS
#define CIF_MACROS
/*
Written by Matthew Rowles.
Inspiration taken from topas.inc and http://topas.dur.ac.uk/topaswiki/doku.php?id=out_out_general_cif_test

The macros to be used by the user are:
* Out_pdCIF2,
* Out_pdCIF2_multi, and/or potentially,
* Out_pdCIF2_per_xdd.

To see their definitions, see the end of this file.

In general:

macro Out_pdCIF2_per_xdd(ciffile)                     { Out_pdCIF2_per_xdd(ciffile, "proc", =Get(bkg);) }
macro Out_pdCIF2_per_xdd(ciffile, data_type)          { Out_pdCIF2_per_xdd(ciffile, data_type, =Get(bkg);) }
macro Out_pdCIF2_per_xdd(ciffile, data_type, bkq_eqn) {

Out_pdCIF2_per_xdd outputs diffraction data and crystal structure information for the xdd under which it is called.

The macro takes three arguments:
ciffile: the name of the file you want to write to. The data is appended.
data_type: "meas" - data as-measured; or "proc" - data has been processed before analysis
bkg_eqn: an equation defining your background function so it can be ouput in the CIF

Two helper macros are given if you only wish to call the macro with one or two arguments. The remaining
arguments are automaticaly set as shown.

###############

macro Out_pdCIF2(ciffile)                     { Out_pdCIF2(ciffile, "proc", =Get(bkg);) }
macro Out_pdCIF2(ciffile, data_type)          { Out_pdCIF2(ciffile, data_type, =Get(bkg);) }
macro Out_pdCIF2(ciffile, data_type, bkq_eqn) { Out_pdCIF2(ciffile, data_type, bkq_eqn, , ) }
macro Out_pdCIF2(ciffile, data_type, bkq_eqn, n, m) {

Out_pdCIF outputs diffraction data and crystal structure information for all xdds given in the macro call.
It is meant for use in situations where each xdd is independent of the other xdds.

The macro takes five arguments:
ciffile: the name of the file you want to write to. The data is appended.
data_type: "meas" - data as-measured; or "proc" - data has been processed before analysis
bkg_eqn: an equation defining your background function so it can be ouput in the CIF
n: the inclusive number of the xdd at which to start the pdCIF output
m: the inclusive number of the xdd at which to stop the pdCIF output

The macro with five arguments would be called rarely, and exits for completeness. Normally, the helper
macro with three arguments would be called; this outputs all xdds and all strs in those xdds.

Three helper macros are given if you only wish to call the macro with one, two, or three arguments. The
remaining arguments are automaticaly set as shown.

###############

macro Out_pdCIF2_multi(ciffile)                     { Out_pdCIF2_multi(ciffile, "proc", =Get(bkg);) }
macro Out_pdCIF2_multi(ciffile, data_type)          { Out_pdCIF2_multi(ciffile, data_type, =Get(bkg);) }
macro Out_pdCIF2_multi(ciffile, data_type, bkg_eqn) { Out_pdCIF2_multi(ciffile, data_type, bkg_eqn, 1, ) }
macro Out_pdCIF2_multi(ciffile, data_type, bkg_eqn, n, m) {

Out_pdCIF2_multi outputs diffraction data and crystal structure information for all xdds given in the macro call.
It is meant for use in situations where some xdds are dependent on other xdds. eg multi-bank tof refinements

The macro takes five arguments:
ciffile: the name of the file you want to write to. The data is appended.
data_type: "meas" - data as-measured; or "proc" - data has been processed before analysis
bkg_eqn: an equation defining your background function so it can be ouput in the CIF
n: the inclusive number of the xdd at which to start the pdCIF output
m: the inclusive number of the xdd at which to stop the pdCIF output

The macro with five arguments would be called in a situation where there are many groups of xdds in a
single inp file. The macro would be called as many times as there are groups, with the start and stop xdd
numbers given in each macro call. It is assumed that each group of xdds is contiguous in the inp file.
In there event of having only a single group in the inp file, the three-argument version can be used.

Three helper macros are given if you only wish to call the macro with one, two, or three arguments. The
remaining arguments are automaticaly set as shown.

###############

Read the first sections for information only. Unless you want to build
your own CIF file output. Best head on to the end of this file for the
actual macros to use (and the comments immediately preceeding).

To get maximum benefit, copy this file to your TOPAS directory and add
the following line to your local.inc:
#include cif.inc

For updates, check http://topas.dur.ac.uk/topaswiki/doku.php?id=out_pdcif

-----------

MIT License

Copyright (c) 2025 Matthew Rowles

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

'##############################################################'
'Helper macros'
macro TOF_ED_CW(tof,ed,cw) {
/*
This macro allows you to choose between three different things depending
on whether or not the data is TOF, EDXRD, or constant wavelength
*/
	if And(Obj_There(neutron), Or(Prm_There(pk_xo_exists), Obj_There(pk_xo))) { 'neutron TOF'
		tof
	} else if Obj_There(d_spacing_to_energy_in_eV_for_f1_f11) { 'energy-dispersive X-ray'
		ed
	} else { 'constant wavelength X-ray or neutron'
		cw
	}
}

macro & d_spacing_from_TOF(& t0, & t1, & t2) {
' This macro calculates d spacing from TOF'
	(-t1 + Sqrt(t1^2 - 4 t2 (t0 - X))) / (2 t2)
}

macro & d_spacing_from_EDXRD(& th2) { 'degrees'
' This macro calculates d spacing from EDXRD - th2 is the angle of the detector in degrees'
	12398.41974 / ( 2 X Sin(th2 Deg_on_2)) ' X is eV'
	'= (h c / e) / (1x10^-10) -> eV per angstrom'
}

macro n_to_m(n,m) {
	#m_ifarg n ""
		'do nothing'
	#m_else
			#m_ifarg m ""
				'do nothing'
			#m_else
				n## to ##m
			#m_endif
	#m_endif
}

'##############################################################'
' Macros common to diffraction data and crystal structures'
'##############################################################'

macro Out_CIF2_newfile(ciffile) {
'Opens a file for appending to.'
	out ciffile append
}

macro Out_CIF2_section_break(comment) {
'Inserts a comment in the style of a section break'
	Out_String("\n#######  ")
	Out_String(comment)
	Out_String("  #######")
}

macro Out_CIF2_datablock(blocknamebase, id) {
/*
Create the start of an entry in a CIF file. All CIF files must contain at least
one datablock. The name must be unique.

blocknamebase: a string that you want to appear in the datablock name. No spaces!
id: an identifier to be appended to the blocknamebase to ensure the
    datablock name is unique. No spaces!
*/
	Out_String("\ndata_")
	Out(blocknamebase, "%s")
	Out(id, "_%V")
}
macro Out_CIF2_datablock_dataname {
	Out_String("\ndata_")
}

macro Out_unique_phase_id(id) {
	if Prm_There(CIF_PHASE_ID) {
		Out(CIF_PHASE_ID, "%s")
	} else {
		Out(Get(phase_name), "%s")
		Out(id, "_%V")
	}
}

macro Out_unique_diffractogram_id(id) {
	if Prm_There(CIF_DIFFRACTOGRAM_ID) {
		Out(CIF_DIFFRACTOGRAM_ID, "%s")
	} else {
		Out(Get(xdd_path_name), "%s")
		Out(id, "_%V")
	}
}

macro Out_CIF2_audit_dataset {
    if Prm_There(CIF_AUDIT_DATASET) { Out_String("\n_audit_dataset.id\t") Out(CIF_AUDIT_DATASET, "%s") }
}

'##############################################################'
' ID macros for phase blocks'
'##############################################################'

macro Out_pdCIF2_phaseblockIDs(id) {
    Out_CIF2_audit_dataset
    Out_String("\n\n_pd_phase.id\t")      Out_unique_phase_id(id)
	Out_pdCIF2_phase_name
    Out_String("\n_model.id\t")         if Prm_There(CIF_MODEL_ID)       { Out(CIF_MODEL_ID, "%s") }       else { Out_unique_phase_id(id) }
    Out_String("\n_structure.id\t")     if Prm_There(CIF_STRUCTURE_ID)   { Out(CIF_STRUCTURE_ID, "%s") }   else { Out_unique_phase_id(id) }
    Out_String("\n_space_group.id\t")   if Prm_There(CIF_SPACE_GROUP_ID) { Out(CIF_SPACE_GROUP_ID, "%s") } else { Out_unique_phase_id(id) }

    Out_String("\n_model.structure_id\t")       if Prm_There(CIF_STRUCTURE_ID)   { Out(CIF_STRUCTURE_ID, "%s") }   else { Out_unique_phase_id(id) }
    if Prm_There(CIF_DIFFRN_ID) { Out_String("\n_structure.diffrn_id\t") Out(CIF_DIFFRN_ID, "%s") }
    Out_String("\n_structure.space_group_id\t") if Prm_There(CIF_SPACE_GROUP_ID) { Out(CIF_SPACE_GROUP_ID, "%s") } else { Out_unique_phase_id(id) }


    Out_String("\n")
}

macro Out_CIF2_free_text(text) {
'Inserts a string into the CIF. Must conform to CIF standards'
	Out_String(text)
}


'##############################################################'
' Macros for wavelengths                                       '
'##############################################################'

macro Out_CIF_wavelength( yminmax,
                         la0, lo0, lh0, lg0) { Out_CIF_wavelength( yminmax, la0, lo0, lh0, lg0, , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , ) }

macro Out_CIF_wavelength( yminmax,
                         la0, lo0, lh0, lg0,
                         la1, lo1, lh1, lg1) { Out_CIF_wavelength( yminmax, la0, lo0, lh0, lg0, la1, lo1, lh1, lg1, , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , ) }

macro Out_CIF_wavelength( yminmax,
                         la0, lo0, lh0, lg0,
                         la1, lo1, lh1, lg1,
                         la2, lo2, lh2, lg2) { Out_CIF_wavelength( yminmax, la0, lo0, lh0, lg0, la1, lo1, lh1, lg1, la2, lo2, lh2, lg2, , , , , , , , , , , , , , , , , , , , , , , , , , , , ) }

macro Out_CIF_wavelength( yminmax,
                         la0, lo0, lh0, lg0,
                         la1, lo1, lh1, lg1,
                         la2, lo2, lh2, lg2,
                         la3, lo3, lh3, lg3) { Out_CIF_wavelength( yminmax, la0, lo0, lh0, lg0, la1, lo1, lh1, lg1, la2, lo2, lh2, lg2, la3, lo3, lh3, lg3, , , , , , , , , , , , , , , , , , , , , , , , ) }

macro Out_CIF_wavelength( yminmax,
                         la0, lo0, lh0, lg0,
                         la1, lo1, lh1, lg1,
                         la2, lo2, lh2, lg2,
                         la3, lo3, lh3, lg3,
                         la4, lo4, lh4, lg4) { Out_CIF_wavelength( yminmax, la0, lo0, lh0, lg0, la1, lo1, lh1, lg1, la2, lo2, lh2, lg2, la3, lo3, lh3, lg3, la4, lo4, lh4, lg4, , , , , , , , , , , , , , , , , , , , ) }

macro Out_CIF_wavelength( yminmax,
                         la0, lo0, lh0, lg0,
                         la1, lo1, lh1, lg1,
                         la2, lo2, lh2, lg2,
                         la3, lo3, lh3, lg3,
                         la4, lo4, lh4, lg4,
                         la5, lo5, lh5, lg5) { Out_CIF_wavelength( yminmax, la0, lo0, lh0, lg0, la1, lo1, lh1, lg1, la2, lo2, lh2, lg2, la3, lo3, lh3, lg3, la4, lo4, lh4, lg4, la5, lo5, lh5, lg5, , , , , , , , , , , , , , , , ) }

macro Out_CIF_wavelength( yminmax,
                         la0, lo0, lh0, lg0,
                         la1, lo1, lh1, lg1,
                         la2, lo2, lh2, lg2,
                         la3, lo3, lh3, lg3,
                         la4, lo4, lh4, lg4,
                         la5, lo5, lh5, lg5,
                         la6, lo6, lh6, lg6) { Out_CIF_wavelength( yminmax, la0, lo0, lh0, lg0, la1, lo1, lh1, lg1, la2, lo2, lh2, lg2, la3, lo3, lh3, lg3, la4, lo4, lh4, lg4, la5, lo5, lh5, lg5, la6, lo6, lh6, lg6, , , , , , , , , , , , ) }

macro Out_CIF_wavelength( yminmax,
                         la0, lo0, lh0, lg0,
                         la1, lo1, lh1, lg1,
                         la2, lo2, lh2, lg2,
                         la3, lo3, lh3, lg3,
                         la4, lo4, lh4, lg4,
                         la5, lo5, lh5, lg5,
                         la6, lo6, lh6, lg6,
                         la7, lo7, lh7, lg7) { Out_CIF_wavelength( yminmax, la0, lo0, lh0, lg0, la1, lo1, lh1, lg1, la2, lo2, lh2, lg2, la3, lo3, lh3, lg3, la4, lo4, lh4, lg4, la5, lo5, lh5, lg5, la6, lo6, lh6, lg6, la7, lo7, lh7, lg7, , , , , , , , ) }

macro Out_CIF_wavelength( yminmax,
                         la0, lo0, lh0, lg0,
                         la1, lo1, lh1, lg1,
                         la2, lo2, lh2, lg2,
                         la3, lo3, lh3, lg3,
                         la4, lo4, lh4, lg4,
                         la5, lo5, lh5, lg5,
                         la6, lo6, lh6, lg6,
                         la7, lo7, lh7, lg7,
                         la8, lo8, lh8, lg8) { Out_CIF_wavelength( yminmax, la0, lo0, lh0, lg0, la1, lo1, lh1, lg1, la2, lo2, lh2, lg2, la3, lo3, lh3, lg3, la4, lo4, lh4, lg4, la5, lo5, lh5, lg5, la6, lo6, lh6, lg6, la7, lo7, lh7, lg7, la8, lo8, lh8, lg8, , , , ) }

macro Out_CIF_wavelength( yminmax,
                         la0, lo0, lh0, lg0,
                         la1, lo1, lh1, lg1,
                         la2, lo2, lh2, lg2,
                         la3, lo3, lh3, lg3,
                         la4, lo4, lh4, lg4,
                         la5, lo5, lh5, lg5,
                         la6, lo6, lh6, lg6,
                         la7, lo7, lh7, lg7,
                         la8, lo8, lh8, lg8,
                         la9, lo9, lh9, lg9) {
    lam
        ymin_on_ymax yminmax

        la la0
        lo lo0
        #m_ifarg lh0 "" #m_else lh lh0 #m_endif
        #m_ifarg lg0 "" #m_else lg lg0 #m_endif

        #m_ifarg la1 "" #m_else la la1 #m_endif
        #m_ifarg lo1 "" #m_else lo lo1 #m_endif
        #m_ifarg lh1 "" #m_else lh lh1 #m_endif
        #m_ifarg lg1 "" #m_else lg lg1 #m_endif

        #m_ifarg la2 "" #m_else la la2 #m_endif
        #m_ifarg lo2 "" #m_else lo lo2 #m_endif
        #m_ifarg lh2 "" #m_else lh lh2 #m_endif
        #m_ifarg lg2 "" #m_else lg lg2 #m_endif

        #m_ifarg la3 "" #m_else la la3 #m_endif
        #m_ifarg lo3 "" #m_else lo lo3 #m_endif
        #m_ifarg lh3 "" #m_else lh lh3 #m_endif
        #m_ifarg lg3 "" #m_else lg lg3 #m_endif

        #m_ifarg la4 "" #m_else la la4 #m_endif
        #m_ifarg lo4 "" #m_else lo lo4 #m_endif
        #m_ifarg lh4 "" #m_else lh lh4 #m_endif
        #m_ifarg lg4 "" #m_else lg lg4 #m_endif

        #m_ifarg la5 "" #m_else la la5 #m_endif
        #m_ifarg lo5 "" #m_else lo lo5 #m_endif
        #m_ifarg lh5 "" #m_else lh lh5 #m_endif
        #m_ifarg lg5 "" #m_else lg lg5 #m_endif

        #m_ifarg la6 "" #m_else la la6 #m_endif
        #m_ifarg lo6 "" #m_else lo lo6 #m_endif
        #m_ifarg lh6 "" #m_else lh lh6 #m_endif
        #m_ifarg lg6 "" #m_else lg lg6 #m_endif

        #m_ifarg la7 "" #m_else la la7 #m_endif
        #m_ifarg lo7 "" #m_else lo lo7 #m_endif
        #m_ifarg lh7 "" #m_else lh lh7 #m_endif
        #m_ifarg lg7 "" #m_else lg lg7 #m_endif

        #m_ifarg la8 "" #m_else la la8 #m_endif
        #m_ifarg lo8 "" #m_else lo lo8 #m_endif
        #m_ifarg lh8 "" #m_else lh lh8 #m_endif
        #m_ifarg lg8 "" #m_else lg lg8 #m_endif

        #m_ifarg la9 "" #m_else la la9 #m_endif
        #m_ifarg lo9 "" #m_else lo lo9 #m_endif
        #m_ifarg lh9 "" #m_else lh lh9 #m_endif
        #m_ifarg lg9 "" #m_else lg lg9 #m_endif


	Out_CIF2_newfile(ciffile)
	Out_String("\n")
	Out_CIF2_section_break("Begin wavelength information")
	Out_CIF2_datablock_dataname Out_unique_diffractogram_id(id) Out_String("_") Out_String("wavelength")
    if Prm_There(CIF_RADIATION_ID) { Out_String("\n_diffrn_radiation.id\t") Out(CIF_RADIATION_ID, "%s") }

    Out_String("\nloop_")
    Out_String("\n_diffrn_radiation_wavelength.id")
    Out_String("\n_diffrn_radiation_wavelength.weight")
    Out_String("\n_diffrn_radiation_wavelength.value")
                            Out("0", "\n%s\t") Out(la0, "%11.7f\t") Out(lo0, "%11.7f")
    #m_ifarg la1 "" #m_else Out("1", "\n%s\t") Out(la1, "%11.7f\t") Out(lo1, "%11.7f") #m_endif
    #m_ifarg la2 "" #m_else Out("2", "\n%s\t") Out(la2, "%11.7f\t") Out(lo2, "%11.7f") #m_endif
    #m_ifarg la3 "" #m_else Out("3", "\n%s\t") Out(la3, "%11.7f\t") Out(lo3, "%11.7f") #m_endif
    #m_ifarg la4 "" #m_else Out("4", "\n%s\t") Out(la4, "%11.7f\t") Out(lo4, "%11.7f") #m_endif
    #m_ifarg la5 "" #m_else Out("5", "\n%s\t") Out(la5, "%11.7f\t") Out(lo5, "%11.7f") #m_endif
    #m_ifarg la6 "" #m_else Out("6", "\n%s\t") Out(la6, "%11.7f\t") Out(lo6, "%11.7f") #m_endif
    #m_ifarg la7 "" #m_else Out("7", "\n%s\t") Out(la7, "%11.7f\t") Out(lo7, "%11.7f") #m_endif
    #m_ifarg la8 "" #m_else Out("8", "\n%s\t") Out(la8, "%11.7f\t") Out(lo8, "%11.7f") #m_endif
    #m_ifarg la9 "" #m_else Out("9", "\n%s\t") Out(la9, "%11.7f\t") Out(lo9, "%11.7f") #m_endif

	Out_CIF2_section_break("End wavelength information")
}

macro Out_CIF_wavelength_coka3 {
Out_CIF_wavelength(0.0001,
      0.5981,1.788960,0.4700,,
      0.0503,1.789879,0.3357,,
      0.3516,1.792850,0.8052,)
}
macro Out_CIF_wavelength_crka7_holzer {
Out_CIF_wavelength(0.0001,
      0.378, 2.2896977, 0.6160974,,
      0.132, 2.2900254, 0.7444350,,
      0.084, 2.2905983, 1.3279580,,
      0.073, 2.2915136, 2.1807273,,
      0.009, 2.2882482, 0.8395685,,
      0.271, 2.2936467, 0.9436726,,
      0.054, 2.2943110, 2.0124101,)
}
macro Out_CIF_wavelength_crkb5_holzer {
    Out_CIF_wavelength(0.0001,
      0.307, 2.0848200, 0.5959633,,
      0.236, 2.0889262, 5.6241545,,
      0.172, 2.0850864, 0.6662470,,
      0.148, 2.0865602, 2.3492088,)
}
macro Out_CIF_wavelength_cuk1sharp {
    Out_CIF_wavelength(0.0001,1,1.540596,0.401844,)
}
macro Out_CIF_wavelength_cuka1 {
    Out_CIF_wavelength(0.0001,1,1.540596,0.501844,)
)
}
macro Out_CIF_wavelength_cuka2 {
    Out_CIF_wavelength(0.0001,
      0.653817,1.540596,0.501844,,
      0.346183,1.544493,0.626579,)
}
macro Out_CIF_wavelength_cuka2_analyt {
    Out_CIF_wavelength(0.0001,
      0.653817,1.540598,0.5,,
      0.346183,1.544426,0.5,)
}
macro Out_CIF_wavelength_cuka4_holzer {
    Out_CIF_wavelength(0.0001,
      0.579, 1.5405909, 0.4374157,,
      0.080, 1.5410639, 0.6432140,,
      0.236, 1.5443990, 0.5128764,,
      0.105, 1.5446855, 0.6872322,)
}
macro Out_CIF_wavelength_cuka5 {
    Out_CIF_wavelength(0.0001,
		0.5791, 1.540596, 0.437,,
		0.0159, 1.534753, 3.6854,,
		0.0762, 1.541058, 0.6,,
		0.2417, 1.54441 , 0.52,,
		0.0871, 1.544721, 0.62,)
}
macro Out_CIF_wavelength_cuka5_berger {
    Out_CIF_wavelength(0.0001,
      0.5691, 1.540596, 0.4370,,
      0.0159, 1.534753, 3.6854,,
      0.0762, 1.541058, 0.6000,,
      0.2517, 1.544410, 0.5200,,
      0.0871, 1.544721, 0.6200,)
}
macro Out_CIF_wavelength_cukb4_holzer {
    Out_CIF_wavelength(0.0001,
		0.485, 1.3922160, 0.5502872,,
		0.248, 1.3925949, 0.5505868,,
		0.110, 1.3917581, 0.5546122,,
		0.100, 1.3934905, 1.2654733,,
		0.055, 1.3913004, 0.8290293,)
}
macro Out_CIF_wavelength_feka7_holzer {
    Out_CIF_wavelength(0.0001,
      0.278, 1.9359990, 0.4876162,,
      0.182, 1.9362569, 0.5941855,,
      0.106, 1.9370562, 1.4626310,,
      0.094, 1.9366253, 0.8479062,,
      0.207, 1.9399242, 0.7548816,,
      0.066, 1.9405570, 0.7104222,,
      0.065, 1.9402020, 1.3459384,)
}
macro Out_CIF_wavelength_fekb4_holzer {
    Out_CIF_wavelength(0.0001,
		0.301, 1.7594154, 3.5378594,,
		0.279, 1.7568450, 0.7767031,,
		0.241, 1.7565588, 0.4902585,,
		0.179, 1.7574577, 1.5893663,)
}
macro Out_CIF_wavelength_mnka7_holzer {
    Out_CIF_wavelength(0.0001,
      0.353, 2.1018365, 0.6110764,,
      0.141, 2.1021879, 0.7281904,,
      0.079, 2.1032712, 1.6052406,,
      0.066, 2.1026638, 0.9496080,,
      0.005, 2.1016355, 0.3452010,,
      0.229, 2.1058026, 0.8444323,,
      0.110, 2.1062490, 1.5085288,)
}
macro Out_CIF_wavelength_mnkb5_holzer {
    Out_CIF_wavelength(0.0001,
		0.254, 1.9101270, 0.5385290,,
		0.234, 1.9114758, 2.7701238,,
		0.234, 1.9140076, 3.9061842,,
		0.164, 1.9103713, 0.5327797,,
		0.114, 1.9107334, 0.8274467,)
}
macro Out_CIF_wavelength_moka2 {
    Out_CIF_wavelength(0.0001,
			0.6533, 0.709300, 0.2695,,
			0.3467, 0.713574, 0.2795,)
}
macro Out_CIF_wavelength_nika5_holzer {
    Out_CIF_wavelength(0.0001,
      0.487, 1.6579244, 0.4462793,,
      0.171, 1.6583129, 1.0449117,,
      0.250, 1.6617352, 0.5955505,,
      0.064, 1.6620153, 0.6770710,,
      0.028, 1.6624264, 0.9977195,)
}
macro Out_CIF_wavelength_nikb4_holzer {
    Out_CIF_wavelength(0.0001,
		0.450, 1.5001100, 0.6824449,,
		0.258, 1.5004731, 0.7880970,,
		0.203, 1.5016253, 2.4915953,,
		0.089, 1.4994406, 0.9393379,)
}
'macro Out_CIF_wavelength_ {
'    Out_CIF_wavelength(
')
'}


'##############################################################'
' Macros for preferred orientation                             '
'##############################################################'




macro PO_Two_Directions_simple(
   & c1, ang1, hkl1,
   & c2, ang2, hkl2,
   & w1c
   )
{
   #m_ifarg ang1 ""
      #m_unique_not_refine ang1
   #m_endif
   #m_ifarg ang2 ""
      #m_unique_not_refine ang2
   #m_endif

	march_dollase ang1 hkl1 = c1;
	march_dollase ang2 hkl2 = c2;
	scale_pks = w1c ang1 + (1-w1c) ang2;
}

macro PO_Three_Directions_simple(
	& c1, ang1, hkl1,
	& c2, ang2, hkl2,
	& c3, ang3, hkl3,
	& w1c,
	& w2c
	)
{
	#m_ifarg ang1 ""
		#m_unique_not_refine ang1
	#m_endif
	#m_ifarg ang2 ""
		#m_unique_not_refine ang2
	#m_endif
	#m_ifarg ang3 ""
		#m_unique_not_refine ang3
	#m_endif


	march_dollase ang1 hkl1 = c1;
	march_dollase ang2 hkl2 = c2;
	march_dollase ang3 hkl3 = c3;

	scale_pks = w1c ang1 + w2c ang2 + (1-w1c-w2c) ang3;
}

macro GLOBAL_PO_LIMITS { min 0.5 max 2 }

'	Add_PO_Two_Directions(po1, 1, #h #k #l , po2, 1, #h #k #l, mix, 0.5)
macro Add_PO_Two_Directions(po1, po1_v, hkl1, po2, po2_v, hkl2, mix, mix_v) {
   	#m_argu po1
   	#m_argu po2
   	#m_argu mix
		If_Local_Eqn_Rpt(po1, po1_v, GLOBAL_PO_LIMITS)
		If_Local_Eqn_Rpt(po2, po2_v, GLOBAL_PO_LIMITS)
		If_Local_Eqn_Rpt(mix, mix_v, min 0 max 1)
		PO_Two_Directions_simple(CeV(po1, po1_v),, hkl1, CeV(po2, po2_v),, hkl2, CeV(mix, mix_v))
}

'	Add_PO_Three_Directions(po1, 1, #h #k #l, po2, 1, #h #k #l, po3, 1, #h #k #l, mix1, 0.3, mix2, 0.3, 0.4)
macro Add_PO_Three_Directions(po1, po1_v, hkl1, po2, po2_v, hkl2, po3, po3_v, hkl3, mix1, mix1_v, mix2, mix2_v, mix3_v) {
		#m_argu po1
		#m_argu po2
		#m_argu po3
		#m_argu mix1
		#m_argu mix2

		If_Local_Eqn_Rpt(po1, po1_v, GLOBAL_PO_LIMITS)
		If_Local_Eqn_Rpt(po2, po2_v, GLOBAL_PO_LIMITS)
		If_Local_Eqn_Rpt(po3, po3_v, GLOBAL_PO_LIMITS)

		If_Local_Eqn_Rpt(mix1, mix1_v, min 0.0001 max = Min(1, 1 - mix1);)
		If_Local_Eqn_Rpt(mix2, mix2_v, min 0.0001 max = Min(1, 1 - mix2);)

		local = 1 - CeV(mix1, mix1_v) - CeV(mix2, mix2_v); : mix3_v

		PO_Three_Directions_simple(CeV(po1, po1_v),, hkl1,
		                           CeV(po2, po2_v),, hkl2,
		                           CeV(po3, po3_v),, hkl3,
		                           CeV(mix1, mix1_v), CeV(mix2, mix2_v)
		                           )
}

'macro PO_Spherical_Harmonics(sh, order)
'{
'   #m_ifarg sh ""
'      #m_unique_not_refine sh
'   #m_endif
'   spherical_harmonics_hkl sh
'      sh_order order
'   scale_pks = sh;
'}


' March-Dollase Preferred orientation correction for asymmetric reflection, capillary transmission, or symmetric reflection
'Ida, Takeshi. 2013. Effect of Preferred Orientation in Synchrotron X-Ray Powder Diffraction. Annual Report of the Advanced Ceramics Research Center: Nagoya Institute of Technology. http://www.crl.nitech.ac.jp/ar/2013/0711_acrc_ar2013_review.pdf.
'angles in radians
macro & cosrho(& alpha, & Delta, & phi)
{
	Cos(alpha)*Cos(Delta) - Sin(alpha)*Sin(Delta)*Sin(phi)
}

macro & PO_P(& r, & alpha, & Delta, & phi)
{
	(r^2 * cosrho(alpha,Delta,phi)^2 + (1-cosrho(alpha,Delta,phi)^2)/r)^(-3/2)
}

'this is equation 10 in Takashi's paper
macro & PO_f(& r, & alpha, & Delta)
{
	'I've hard coded this for N==64, as the sum thing doesn't work.
	'(1/N)*Sum( PO_P(r,alpha,Delta,(j+(1/2))*Pi/N) ,j=0,j<=N-1,j=j+1)
	 (1/64)*
	 (PO_P(r,alpha,Delta,( 0+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,( 1+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,( 2+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,( 3+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,( 4+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,( 5+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,( 6+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,( 7+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,( 8+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,( 9+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(10+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(11+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(12+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(13+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(14+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(15+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(16+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(17+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(18+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(19+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(20+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(21+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(22+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(23+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(24+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(25+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(26+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(27+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(28+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(29+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(30+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(31+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(32+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(33+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(34+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(35+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(36+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(37+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(38+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(39+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(40+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(41+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(42+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(43+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(44+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(45+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(46+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(47+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(48+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(49+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(50+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(51+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(52+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(53+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(54+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(55+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(56+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(57+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(58+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(59+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(60+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(61+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(62+0.5)*Pi/64) +
	  PO_P(r,alpha,Delta,(63+0.5)*Pi/64))
}


'angles in radians
macro generalised_PO_eqn(r_c, r_v, alpha, hkl, Delta_c, Delta_v)
{
   #m_argu r_c
   #m_argu Delta_c
   If_Prm_Eqn_Rpt(r_c, r_v, min 0.0001 max = 2 Val + .5;) 'preferred orientation parameter
   If_Prm_Eqn_Rpt(Delta_c, Delta_v, min 0.0001 max =Pi;) 'angle in radians:  ==0 symmetric reflection, ==Pi/2 capillary transmission, ==Abs(Th-Omega) asymmetric reflection

	#m_ifarg alpha ""
      #m_unique_not_refine alpha
    #m_endif
    str_hkl_angle alpha hkl 'angle in radians between PO direction and current diffraction vector
    scale_pks = Multiplicities_Sum(  PO_f(CeV(r_c,r_v), alpha, CeV(Delta_c,Delta_v))  );
}

macro PO_symmetric_reflection(r_c, r_v, hkl)
{
	generalised_PO_eqn(r_c, r_v, , hkl, , 0)
}

macro PO_capillary_transmission(r_c, r_v, hkl)
{
	generalised_PO_eqn(r_c, r_v, , hkl, , (Pi/2))
}

macro PO_asymmetric_reflection(r_c, r_v,omega_v, hkl) { PO_asymmetric_reflection(r_c, r_v,,omega_v, hkl) }
macro PO_asymmetric_reflection(r_c, r_v,omega_c,omega_v, hkl)
{
	#m_argu omega_c
	If_Prm_Eqn_Rpt(omega_c, omega_v, min 0.0001 max 90) 'incident angle in degrees
	prm #m_unique Delta = Abs(Th - ((CeV(omega_c,omega_v))*Deg)) ;
	generalised_PO_eqn(r_c, r_v, , hkl, , Delta)
}

macro PO_asymmetric_reflection_special(r_c, r_v,omega_c,omega_v, hkl,f)
{
	#m_argu omega_c
	If_Prm_Eqn_Rpt(omega_c, omega_v, min 0.0001 max 90) 'incident angle in degrees
	prm #m_unique Delta = Abs(Th - ((CeV(omega_c,omega_v))*Deg)) ;
	generalised_PO_eqn_special(r_c, r_v, , hkl, , Delta,f)
}
macro generalised_PO_eqn_special(r_c, r_v, alpha, hkl, Delta_c, Delta_v,f)
{
   #m_argu r_c
   #m_argu Delta_c
   If_Prm_Eqn_Rpt(r_c, r_v, min 0.0001 max = 2 Val + .5;) 'preferred orientation parameter
   If_Prm_Eqn_Rpt(Delta_c, Delta_v, min 0.0001 max =Pi;) 'angle in radians:  ==0 symmetric reflection, ==Pi/2 capillary transmission, ==Abs(Th-Omega) asymmetric reflection

	#m_ifarg alpha ""
      #m_unique_not_refine alpha
   #m_endif

   str_hkl_angle alpha hkl 'angle in radians between PO direction and current diffraction vector

   scale_pks = f Multiplicities_Sum(  PO_f(CeV(r_c,r_v), alpha, CeV(Delta_c,Delta_v))  );
}


'##############################################################'
' Macros for diffraction data                                  '
'##############################################################'

macro Out_pdCIF2_scan_method(method) {
/*
the value of "method" you pass in must be one of:-
step :	step scan
cont :	continuous scan
tof  :	time of flight
disp :	energy dispersive
fixed:	stationary detector
*/
	#m_ifarg method "" #m_else
		Out(method, "\n_pd_meas.scan_method\t%s")
	#m_endif
}

macro Out_pdCIF2_instr_special_details(details) {
	Out(details, "\n_pd_instr.special_details\n;%s\n;")
}

macro Out_pdCIF2_datetime_initiated(datetime) {
/*
   The date and time of the data-set measurement. Entries follow
   the standard CIF format 'yyyy-mm-ddThh:mm:ss+zz'. Use
   of seconds and a time zone is optional, but use of hours
   and minutes is strongly encouraged. Where possible, give the
   time when the measurement was started rather than when
   it was completed.
*/
	Out(datetime, "\n_pd_meas.datetime_initiated\t%s")
}

macro Out_CIF2_temperature(temp) {
/*
   The mean temperature in kelvins at which the intensities were measured.
*/
	Out(temp, "\n_diffrn.ambient_temperature\t%V")
}

macro Out_CIF2_pressure(pres) {
/*
   The mean hydrostatic pressure in kilopascals at which the intensities were measured.
*/
	Out(pres, "\n_diffrn.ambient_pressure\t%V")
}

macro Out_pdCIF2_2theta_fixed(th2) {
/*
   The 2\q diffraction angle in degrees for measurements in a white-beam fixed-angle experiment.
*/
	Out(th2, "\n_pd_meas.2theta_fixed\t%s")
}

macro Out_pdCIF2_phase_mass {
    if Obj_There(spiked_mwp) {
        Out(Get(corrected_weight_percent), "\n_pd_phase_mass.absolute\t%V")
    }
    Out(Get(weight_percent), "\n_pd_phase_mass.percent\t%V")
}

macro Out_pdCIF2_Rfactors {
	Out(Get(gof),       "\n_pd_proc_ls.prof_gof_factor \t%1.5f")
	Out(Get(r_p)/100,   "\n_pd_proc_ls.prof_R_factor   \t%1.5f")
	Out(Get(r_wp)/100,  "\n_pd_proc_ls.prof_wR_factor  \t%1.5f")
	Out(Get(r_exp)/100, "\n_pd_proc_ls.prof_wR_expected\t%1.5f")
}

/*
These next set of groups of three macros are for outputting constant-wavelength angle-dispersive data,
time-of-flight neutron data, and energy-dispersive X-ray data.

meas denotes intensity measurements at the measurement point
proc denotes intensity measurements that have been processed in some way at the measurement point
*/
macro Out_pdCIF2_angle_dispersive_measured_data(ciffile) {
	Out_pdCIF2_angle_dispersive_data(ciffile, meas, =Get(bkg);)
}
macro Out_pdCIF2_angle_dispersive_processed_data(ciffile) {
	Out_pdCIF2_angle_dispersive_data(ciffile, proc, =Get(bkg);)
}
macro Out_pdCIF2_angle_dispersive_data(ciffile, type, bkg_eqn) {
/*
ciffile - string. The name of the cif file you want to write to.
bkg_eqn - topas equation. The equation you arre using for your background.
          If you are using the built in bkg, then write "=Get(bkg);" (without the "")
*/
	Out_CIF2_newfile(ciffile)
	Out_String("\nloop_")
	Out_String("\n\t_pd_data.point_id")
	Out_String("\n\t_pd_meas.2theta_scan")
	#if type == "meas";
		Out_String("\n\t_pd_meas.intensity_total")
	#else
		Out_String("\n\t_pd_proc.intensity_total")
	#endif
	Out_String("\n\t_pd_proc.ls_weight")
	Out_String("\n\t_pd_calc.intensity_total")
	#m_ifarg bkg_eqn ""
        xdd_out ciffile append
            load out_record out_fmt out_eqn {
                "\n%9.0f  "  = Xi;
                "  %11.6f  " = X;
                "  %11.6f  " = Yobs;
                "  %11.6f  " = Get(weighting);
                "  %11.6f  " = Ycalc;
            }
	#m_else
        Out_String("\n\t_pd_calc.intensity_bkg")
        xdd_out ciffile append
            load out_record out_fmt out_eqn {
                "\n%9.0f  "  = Xi;
                "  %11.6f  " = X;
                "  %11.6f  " = Yobs;
                "  %11.6f  " = Get(weighting);
                "  %11.6f  " = Ycalc;
                "  %11.6f  " bkg_eqn
            }
	#m_endif
}

macro Out_pdCIF2_time_of_flight_measured_data(ciffile) {
	Out_pdCIF2_time_of_flight_data(ciffile, meas, =Get(bkg);)
}
macro Out_pdCIF2_time_of_flight_processed_data(ciffile) {
	Out_pdCIF2_time_of_flight_data(ciffile, proc, =Get(bkg);)
}
macro Out_pdCIF2_time_of_flight_data(ciffile, type, bkg_eqn) {
	Out_CIF2_newfile(ciffile)
	Out_String("\nloop_")
	Out_String("\n\t_pd_data.point_id")
	Out_String("\n\t_pd_meas.time_of_flight")
	Out_String("\n\t_pd_proc.d_spacing")
	#if type == "meas";
		Out_String("\n\t_pd_meas.intensity_total")
	#else
		Out_String("\n\t_pd_proc.intensity_total")
	#endif
	Out_String("\n\t_pd_proc.ls_weight")
	Out_String("\n\t_pd_calc.intensity_total")
	#m_ifarg bkg_eqn ""
		xdd_out ciffile append
			load out_record out_fmt out_eqn {
                "\n%9.0f  "  = Xi;
                "  %11.6f  " = X;
                "  %11.6f  " = d_spacing_from_TOF(t0, t1, t2);
                "  %11.6f  " = Yobs;
                "  %11.6f  " = Get(weighting);
                "  %11.6f  " = Ycalc;
			}
	#m_else
		Out_String("\n\t_pd_calc.intensity_bkg")
		xdd_out ciffile append
			load out_record out_fmt out_eqn {
                "\n%9.0f  "  = Xi;
                "  %11.6f  " = X;
                "  %11.6f  " = d_spacing_from_TOF(t0, t1, t2);
                "  %11.6f  " = Yobs;
                "  %11.6f  " = Get(weighting);
                "  %11.6f  " = Ycalc;
                "  %11.6f  " bkg_eqn
			}
	#m_endif
}


macro Out_pdCIF2_energy_dispersive_measured_data {
	Out_pdCIF2_energy_dispersive_data(meas, =Get(bkg);)
}
macro Out_pdCIF2_energy_dispersive_processed_data {
	Out_pdCIF2_energy_dispersive_data(proc, =Get(bkg);)
}
macro Out_pdCIF2_energy_dispersive_data(ciffile, type, bkg_eqn) {
	Out_CIF2_newfile(ciffile)
	Out_String("\nloop_")
	Out_String("\n\t_pd_data.point_id")
	Out_String("\n\t_pd_proc.energy_incident")
	Out_String("\n\t_pd_proc.d_spacing")
	#if type == "meas";
		Out_String("\n\t_pd_meas.intensity_total")
	#else
		Out_String("\n\t_pd_proc.intensity_total")
	#endif
	Out_String("\n\t_pd_proc.ls_weight")
	Out_String("\n\t_pd_calc.intensity_total")
	#m_ifarg bkg_eqn ""
        xdd_out ciffile append
            load out_record out_fmt out_eqn {
                "\n%9.0f  "  = Xi;
                "  %11.6f  " = X;
                "  %11.6f  " = d_spacing_from_EDXRD(CIF_TH2_FIXED);
                "  %11.6f  " = Yobs;
                "  %11.6f  " = Get(weighting);
                "  %11.6f  " = Ycalc;
            }
	#m_else
        Out_String("\n\t_pd_calc.intensity_bkg")
        xdd_out ciffile append
            load out_record out_fmt out_eqn {
                "\n%9.0f  "  = Xi;
                "  %11.6f  " = X;
                "  %11.6f  " = d_spacing_from_EDXRD(CIF_TH2_FIXED);
                "  %11.6f  " = Yobs;
                "  %11.6f  " = Get(weighting);
                "  %11.6f  " = Ycalc;
                "  %11.6f  " bkg_eqn
            }
	#m_endif
}

macro Out_pdCIF2_calc_component(ciffile) {
/*
ciffile - string. The name of the cif file you want to write to.
*/
	Out_CIF2_newfile(ciffile)
	Out_String("\nloop_")
	Out_String("\n_pd_calc_component.point_id")
	Out_String("\n_pd_calc_component.intensity_net")
        phase_out_X ciffile append
            load out_record out_fmt out_eqn {
                "\n%9.0f  "  = Xi;
                "  %11.6f  " = Get(phase_ycalc);
            }
}

'############################################################## '
' Macros for crystal structures                                 '
'############################################################## '

macro Out_pdCIF2_phase_name {

    if Obj_There(phase_name) {
	    Out(Get(phase_name), "\n_pd_phase.name\t%s")
    }
}

macro Out_CIF2_unit_cell_prms {
	Out(Get(a),  "\n_cell.length_a    %V")
	Out(Get(b),  "\n_cell.length_b    %V")
	Out(Get(c),  "\n_cell.length_c    %V")
	Out(Get(al), "\n_cell.angle_alpha %V")
	Out(Get(be), "\n_cell.angle_beta  %V")
	Out(Get(ga), "\n_cell.angle_gamma %V")
	Out(Get(cell_volume), "\n_cell.volume %V")
}

macro Out_CIF2_Z_molecular_weight(Z) {
	local #m_unique !formula_weight_for_CIF_output = Get(cell_mass) / Z;
	Out(Z, "\n_cell.formula_units_Z\t%3.0f")
	Out(formula_weight_for_CIF_output, "\n_chemical_formula.weight\t%V")
}

macro Out_CIF2_chemical_formula(formula) {
/*
   example
   'C18 H19 N7 O8 S'
*/
	Out(formula, "\n_chemical_formula.sum\n;%s\n;")
}

macro Out_CIF2_diffrn_measurement_device_type(goniotype) {
/*
   The make, model or name of the measurement device (goniometer) used.
*/
	Out(goniotype, "\n_diffrn_measurement.device_make\n;%s\n;")
}

macro Out_CIF2_space_group_crystal_system(system) {
/*
triclinic
monoclinic
orthorhombic
tetragonal
trigonal (use this for rhombohedral)
hexagonal
cubic
*/
	Out(system, "\n_space_group.crystal_system\t%s")
}

macro Out_CIF2_minmax_theta {
/*
   The maximum and minimum theta angles of reflections used to measure the unit cell in degrees.
*/
	Out((X1/2), "\n_cell_measurement.theta_min\t%V")
	Out((X2/2), "\n_cell_measurement.theta_max\t%V")
}

macro Out_CIF2_density {
/*
   Density values calculated from the crystal cell and contents. The
   units are megagrams per cubic metre (grams per cubic centimetre).
*/
	local #m_unique !density_only_for_CIF_output = (1.6605402 Get(cell_mass)) / Get(cell_volume);
	Out(density_only_for_CIF_output, "\n_pd_phase.density_diffrn\t%V")
}

macro Out_CIF2_absorption {
	local #m_unique !density_for_CIF_output = ((1.6605402 Get(cell_mass)) / Get(cell_volume));
	local #m_unique !LAC_for_CIF_output = Get(phase_MAC) density_for_CIF_output / 10;
	Out(LAC_for_CIF_output, "\n_pd_phase.atten_coef_mu_calc\t%V")
	Out(Get(phase_MAC),     "\n_pd_phase.mass_atten_coef_mu_calc\t%V")
}

macro Out_CIF2_space_group_with_id {
	Out(Get(sp_grp_char), "\n_space_group.name_H-M_alt\n;%s\n;")
	Out_String("\nloop_")
	Out_String("\n\t_space_group_symop.id\n\t_space_group_symop.operation_xyz")
	Out(Get(sp_xyzs_txt_with_id),  "%s")
}

macro Out_CIF2_atom_coords(ciffile) {
	Out_CIF2_newfile(ciffile)
	Out_String("\nloop_")
	Out_String("\n\t_atom_site.label")
	Out_String("\n\t_atom_site.type_symbol")
	Out_String("\n\t_atom_site.site_symmetry_multiplicity")
	Out_String("\n\t_atom_site.fract_x")
	Out_String("\n\t_atom_site.fract_y")
	Out_String("\n\t_atom_site.fract_z")
	Out_String("\n\t_atom_site.occupancy")
	Out_String("\n\t_atom_site.B_iso_or_equiv")
	atom_out ciffile append
		load out_record out_fmt out_eqn {
			"\n\t%s" = Get_From_String(Get(current_atom), site);
			" %s" = Get_From_String(Get(current_atom), atom);
			" %3.0f" = Get_From_String(Get(current_atom), num_posns);
			" %V" = Get_From_String(Get(current_atom), x);
			" %V" = Get_From_String(Get(current_atom), y);
			" %V" = Get_From_String(Get(current_atom), z);
			" %V" = Get_From_String(Get(current_atom), occ);
			" %V" = Get_From_String(Get(current_atom), beq);
		}
}


' todo: add _refln.id column
macro Out_CIF2_hkls(ciffile) {
	Out_CIF2_newfile(ciffile)
	Out_String("\nloop_")
	Out_String("\n_refln.id")
	Out_String("\n_refln.index_h")
	Out_String("\n_refln.index_k")
	Out_String("\n_refln.index_l")
	Out_String("\n_refln.d_spacing")
	Out_String("\n_refln.F_squared_calc")
	Out_String("\n_refln.F_squared_meas")
	phase_out ciffile append
		load out_record out_fmt out_eqn {
			 'begin _refln.id
            "\n%1.0f" = H;
			"%1.0f" = K;
			"%1.0f\t" = L;
             'end _refln.id

			" %4.0f" = H;
			" %4.0f" = K;
			" %4.0f" = L;
			" %11.6f" = D_spacing;
			" %12.6f" = I_no_scale_pks;
			" %12.6f" = Iobs_no_scale_pks;
		}
}

macro Out_CIF2_adps(ciffile) {
	Out_CIF2_newfile(ciffile)
	Out_String("\nloop_")
	Out_String("\n\t_atom_site_aniso.label")
	Out_String("\n\t_atom_site_aniso.U_11")
	Out_String("\n\t_atom_site_aniso.U_22")
	Out_String("\n\t_atom_site_aniso.U_33")
	Out_String("\n\t_atom_site_aniso.U_12")
	Out_String("\n\t_atom_site_aniso.U_13")
	Out_String("\n\t_atom_site_aniso.U_23")
	atom_out ciffile append
		load out_record out_fmt out_eqn {
			"\n%s" = Get_From_String(Get(current_atom), site);
			" %V" = Get_From_String(Get(current_atom), u11);
			" %V" = Get_From_String(Get(current_atom), u22);
			" %V" = Get_From_String(Get(current_atom), u33);
			" %V" = Get_From_String(Get(current_atom), u12);
			" %V" = Get_From_String(Get(current_atom), u13);
			" %V" = Get_From_String(Get(current_atom), u23);
		 }
}

macro Out_CIF2_bond_angles(ciffile) {
	Out_CIF2_newfile(ciffile)
	consider_lattice_parameters 'this propagates errors from lattice parameters to the bonds and angles'
	Out(Get(cif_bonds_angles), "%s")
}

macro Out_magCIF_space_group_with_id {
/*
https://www.iucr.org/__data/iucr/cifdic_html/3/MAGNETIC_CIF/Ispace_group_magn.number_BNS.html
_space_group_magn.number_BNS
     See _space_group_magn.number_OG for a description of magnetic
     space groups (MSGs). The Belov-Neronova-Smirnova (BNS) number for
     an MSG is composed of two positive integers separated by a
     period. The first integer lies in the range [1-230] and indicates
     the non-magnetic space group F for MSGs of types 1-3 or the
     non-magnetic space group of the subgroup D for MSGs of type 4.  The
     second integer is sequential over all MSGs associated  with the
     same crystal family.
     There are 1651 distinct equivalence classes of MSGs, each of
     which has a unique BNS number. These equivalence classes are most
     accurately referred to as magnetic space-group "types",
     following the usage in the International Tables for Crystallography.
     But the word "type"  is also commonly used to
     indicate the four-fold classification of MSGs presented above.
     To avoid confusion, the word "type" is only used in the latter
     sense here.

     Analogous tags: symCIF:_space_group.number_IT

     Ref: 'Magnetic Group Tables' by D.B. Litvin at
     http://www.iucr.org/publ/978-0-9553602-2-0. ISO-MAG tables of H.T.
     Stokes and B.J. Campbell at http://iso.byu.edu.

https://www.iucr.org/__data/iucr/cifdic_html/3/MAGNETIC_CIF/Ispace_group_symop_magn_operation.id.html
_space_group_symop_magn_operation.id
     An arbitrary identifier that uniquely labels each symmetry
     operation in a looped list of magnetic space-group symmetry
     operations. Most commonly, a sequence of positive  integers is
     used for this identification.
     The _space_group_symop_magn.id alias provides backwards
     compatibility with the established magCIF prototype.

https://www.iucr.org/__data/iucr/cifdic_html/3/MAGNETIC_CIF/Ispace_group_symop_magn_operation.xyz.html
_space_group_symop_magn_operation.xyz
     A parsable string giving one of the symmetry operations of the
     magnetic space group in algebraic form.  The analogy between
     parsable labels for magnetic and non-magnetic symmetry operations
     is perfect except for the fact that a magnetic symop label ends
     with an additional piece of information ("-1" or "+1") indicating
     that the operation is or is not time-reversed, respectively.
     This tag is intended for use with the BNS-supercell description
     of a magnetic structure.

     Analogous tags: symCIF:_space_group_symop.operation_xyz

     Ref: 'Magnetic Group Tables' by D.B. Litvin at
     http://www.iucr.org/publ/978-0-9553602-2-0. ISO-MAG tables of H.T.
     Stokes and B.J. Campbell at http://iso.byu.edu.

_space_group_symop.magn_operation.mxmymz
I cannot find a definition in the magCIF for this column.

_space_group_symop.magn_operation.timereversal
Topas does not include the time reversal in the xyz output.
It goes in its own column.

*/
	Out(Get(sp_grp_char ), "\n_space_group_magn.number_BNS\n;%s\n;")
	Out_String("\nloop_")
	Out_String("\n\t_space_group_symop_magn_operation.id")
	Out_String("\n\t_space_group_symop_magn_operation.xyz")
	Out_String("\n\t_space_group_symop_magn_operation.mxmymz")
	Out_String("\n\t_space_group_symop_magn_operation.timereversal")
	Out(Get(mag_sp_xyzs_txt_with_id),  "%s")
}

macro Out_mag_CIF_atom_site_moment {
/*
https://www.iucr.org/__data/iucr/cifdic_html/3/MAGNETIC_CIF/Iatom_site_moment.label.html
_atom_site_moment.label

https://www.iucr.org/__data/iucr/cifdic_html/3/MAGNETIC_CIF/Iatom_site_moment.crystalaxis_x.html
_atom_site_moment.crystalaxis_x
     The component of the atom-site magnetic-moment vector parallel to the first
     unit-cell axis.  See _atom_site_moment.crystalaxis.

https://www.iucr.org/__data/iucr/cifdic_html/3/MAGNETIC_CIF/Iatom_site_moment.crystalaxis_y.html
_atom_site_moment.crystalaxis_y
     The component of the atom-site magnetic-moment vector parallel to the second
     unit-cell axis.  See _atom_site_moment.crystalaxis.

https://www.iucr.org/__data/iucr/cifdic_html/3/MAGNETIC_CIF/Iatom_site_moment.crystalaxis_z.html
_atom_site_moment.crystalaxis_z
     The component of the atom-site magnetic-moment vector parallel to the third
     unit-cell axis.  See _atom_site_moment.crystalaxis.
*/
	Out_CIF2_newfile(ciffile)
	Out_String("\nloop_")
	Out_String("\n\t_atom_site_moment.label")
	Out_String("\n\t_atom_site_moment.crystalaxis_x")
	Out_String("\n\t_atom_site_moment.crystalaxis_y")
	Out_String("\n\t_atom_site_moment.crystalaxis_z")
	mag_atom_out ciffile append
		load out_record out_fmt out_eqn {
			"\n%s" = Get_From_String(Get(current_atom), site);
			" %11.5f" = Get(a) Get_From_String(Get(current_atom), mlx);
			" %11.5f" = Get(b) Get_From_String(Get(current_atom), mly);
			" %11.5f" = Get(c) Get_From_String(Get(current_atom), mlz);
		}
}

'##############################################################%####### '
' These are the consolidiated macros to report structures and patterns   '
'###################################################################### '


macro Out_pdCIF2_xdd_setup {
/*
This macro sets up a workaround in how pk_xo behaves differently between v5-6 and v7
It also defines an ID prm, so you do not need to if there is only one xdd to worry about.
*/
	if Or(Obj_There(pk_xo_prm),Obj_There(pk_xo)) { local !pk_xo_exists 0 }
	if Prm_There(CIF_ID) {
		'do nothing'
	} else {
		if Prm_There(CIF_MULTI_DIFFRACTOGRAM_ID) {
			if Obj_There(num_runs) {
				local CIF_ID = Run_Number;
			} else {
				local CIF_ID 1
			}
		} else { 'not a multi-pattern inp file'
			if Obj_There(range) {
				if Obj_There(num_runs) {
					local CIF_ID = 10000 Get(range) + Run_Number;
				} else {
					local CIF_ID = Get(range);
				}
			} else if Obj_There(num_runs) {
				local CIF_ID = Run_Number;
			} else {
				local CIF_ID 1
			}
		}
	}
}

macro Out_pdCIF2_STR(ciffile, id) {
/*
This macro constructs a crystal structure in CIF format that is likely
to pass checkCIF with minimal issues. Probably.

The construction of the phase_id is simply the concatenation of the
phase_name and an identifier, most usually taken as the number of the xdd,
either defined through the use of "Run_Number" if doing a sequential refinement
using #list, or by setting a local variable in each xdd - local !id = 1; (and
copying it and incrementing it in each xdd)

The diffractogram_id of each diffraction pattern is simply the string "pattern_"
with the id appended.

This macro can be placed into every str that you have, or better, placed at the end
of the input file in a `for strs {}` construct.


ciffile: the name of the file you want to write to. The data is appended.
phase_id: a number uniquely identifying a str inside an xdd
id: a number uniquely identifying an xdd

If you want to report molecular weight for a str, you must define a
local parameter defining the number of formula units. ie:
local !CIF_Z = 6;
You must name it "CIF_Z".
*/
	Out_CIF2_newfile(ciffile)
	Out_String("\n")
	Out_CIF2_section_break("Begin powder crystal structure")
	Out_CIF2_datablock_dataname Out_unique_phase_id(id)
    Out_pdCIF2_phaseblockIDs(id)
	Out_CIF2_unit_cell_prms
	if Prm_There(CIF_Z) { Out_CIF2_Z_molecular_weight(CIF_Z) }
	if Prm_There(CIF_SUM_FORMULA) { Out_CIF2_chemical_formula(CIF_SUM_FORMULA) }
	Out_CIF2_density
	TOF_ED_CW(,, Out_CIF2_absorption Out_CIF2_minmax_theta)
	if Prm_There(CIF_CRYSTAL_SYSTEM) { Out_CIF2_space_group_crystal_system(CIF_CRYSTAL_SYSTEM) }
	if Prm_There(CIF_PHASE_TEXT) { Out_CIF2_free_text(CIF_PHASE_TEXT) }
	if Obj_There(mag_sg) {
		Out_magCIF_space_group_with_id
	} else {
		Out_CIF2_space_group_with_id
	}
	Out_CIF2_atom_coords(ciffile)
	if Obj_There(adps) { Out_CIF2_adps(ciffile) }
	if Obj_There(mag_sg) { Out_mag_CIF_atom_site_moment }
	Out_CIF2_newfile(ciffile)
	Out_CIF2_bond_angles(ciffile)
	Out_CIF2_section_break("End powder crystal structure")
	Out_String("\n")
}

macro Out_pdCIF2_phase_diffractogram_data(ciffile, id) {
	Out_CIF2_newfile(ciffile)
	Out_String("\n")
	Out_CIF2_section_break("Begin phase and diffractogram information")
	Out_CIF2_datablock_dataname Out_unique_phase_id(id) Out_String("_") Out_unique_diffractogram_id(id)
    Out_CIF2_audit_dataset
    Out_String("\n_pd_phase.id\t") Out_unique_phase_id(id)
    Out_String("\n_pd_diffractogram.id\t") Out_unique_diffractogram_id(id)
	if Obj_There(str) {
		Out_pdCIF2_phase_mass
		Out_CIF2_hkls(ciffile)
	}
    Out_pdCIF2_calc_component(ciffile)
    Out_CIF2_newfile(ciffile)
	Out_CIF2_section_break("End phase and diffractogram information")
	Out_String("\n")
}





macro Out_pdCIF2_diffraction_data(ciffile, data_type, bkg_eqn, phase_id, id) {
/*
This macro constructs a powder diffraction pattern in CIF format.

The construction of the block_id is simply the concatenation of the
string "pattern" and an identifier, most usually taken as the  number of the xdd,
either defined through the use of "Run_Number" if doing a sequential refinement
using #list, or by setting a local variable in each xdd - local !id = 1; (and
copying it and incrementing it in each xdd)

The implementation of the block_id in this macro is extremely simplistic, and
designed to be easily automated. If you change how the _pd_block_id or
_pd_diffractogram_id is implemented, make sure to change it in all the other
places - see _pd_phase_block_id also.

This macro can be placed into every xdd that you have, or if you have more, placed
at the end of the input file in a `for xdds {}` construct.

ciffile: the name of the file you want to write to. The data is appended.
data_type: "meas" - data as-measured; "proc" - data has been processed before analysis
bkg_eqn: an equation with the bkg you want to record
phase_id: a number uniquely identifiying a str in an xdd
is: a number uniquely identifying an xdd, or group of xdds in the case of a multi-pattern refinement
*/
	Out_CIF2_newfile(ciffile)
	Out_String("\n")
	Out_CIF2_section_break("Begin powder diffraction data")
	Out_CIF2_datablock_dataname Out_unique_diffractogram_id(id)
	Out_pdCIF2_blockid_dataname Out_unique_diffractogram_id(id)
	if Prm_There(CIF_DATETIME) { Out_pdCIF2_datetime_initiated(CIF_DATETIME) }
	Out_String("\n_pd_calc.method\tRietveld")
	if Obj_There(neutron) {
		Out_String("\n_diffrn_radiation.probe\tneutron")
	} else {
		Out_String("\n_diffrn_radiation.probe\tx-ray")
	}
	if Prm_There(CIF_DIFFRACTOMETER_NAME) { Out_pdCIF2_instr_special_details(CIF_DIFFRACTOMETER_NAME) }
	TOF_ED_CW(Out_pdCIF2_scan_method("tof"),
	          Out_pdCIF2_scan_method("disp"),
	          if Prm_There(CIF_SCAN_METHOD) { Out_pdCIF2_scan_method(CIF_SCAN_METHOD) }
	          if Obj_There(lam) { Out_CIF2_wavelength } )
	if Prm_There(CIF_TEMP) { Out_CIF2_temperature(CIF_TEMP) }
	if Prm_There(CIF_PRES) { Out_CIF2_pressure(CIF_PRES) }
	Out_pdCIF2_Rfactors
	if Prm_There(CIF_DIFFRACTOGRAM_TEXT) { Out_CIF2_free_text(CIF_DIFFRACTOGRAM_TEXT) }
	TOF_ED_CW(Out_pdCIF2_time_of_flight_data(ciffile, data_type, bkg_eqn),
	          Out_pdCIF2_energy_dispersive_data(ciffile, data_type, bkg_eqn),
	          Out_pdCIF2_angle_dispersive_data(ciffile, data_type, bkg_eqn))
	Out_CIF2_newfile(ciffile)
	Out_CIF2_section_break("End powder diffraction data")
	Out_String("\n")
}

macro Out_CIF2_magic_number(ciffile) {
'write the CIF version number to file.'
	out ciffile append
	Out_String("#\#CIF_2.0\n")
}
'##############################################################%#####################################     '
'####################################################################################################     '
'######################################################################################################   '
'######################################################################################################   '
'####################################################################################################     '
'####################################################################################################     '

/* The macros to use at the end of your input file (literally)
   to report structures and patterns is just after this comment!

There are some helper macros that have fewer arguments, and make some
choices about what to write in the CIF file for you.

The use of these macros rely on you using some predefined parameters
for specific items. You can use whatever name you want in your own
calculations, and you only need to use them if you want to report on them,
but they need to be defined somewhere with the following names:

-Must be defined (eg local CIF_PHASE_ID 2 )
CIF_ID - (per xdd, or group of xdds) unique identifier to append to block ids to ensure everything is unique
CIF_PHASE_ID - (per str) unique identifier to append to block ids of strs to ensure everything is unique
t0, t1, t2 - (per xdd - only for tof) TOF calibration constants
CIF_TH2_FIXED - (per xdd - only for edxrd) Detector angle in energy-dispersive XRD

-Should be defined
CIF_SCAN_METHOD - (per xdd) "step" - step scan; "cont" continuous scan; "fixed" stationary detector.
									 This is only relevant for CW data.

-Can be defined
CIF_DATETIME	xdd	Date/time when data collection was initiated
CIF_TEMP - (per xdd) Temperature (K) at which data were acquired.
CIF_PRES - (per xdd) Pressure (kPa) at which data were acquired.
CIF_Z - (per str) formula units per unit cell
CIF_SUM_FORMULA - (per str) what is the chemical formula of the unit cell?
CIF_CRYSTAL_SYSTEM - (per str) the crystal system of the space group
CIF_DIFFRACTOMETER_NAME - (per xdd) short description of the diffractometer
(If you use these next two, you are on your own wrt maintaining uniqueness
CIF_DIFFRACTOGRAM_BLOCK_ID	xdd	Fully specified block ID for a diffraction pattern
CIF_PHASE_BLOCK_ID	str	Fully specified block ID for a crystal structure
CIF_DIFFRACTOGRAM_TEXT	xdd	Free-text entry that goes in the data block containing the diffraction data
CIF_PHASE_TEXT	str	Free-text entry that goes in the data block containing the structure


ciffile: the name of the file you want to write to. The data is appended.
data_type: "meas" - data as-measured; or "proc" - data has been processed before analysis
bkg_eqn: an equation defining your background function so it can be ouput in the CIF
n,m: numbers representing the xdds over which the output should be run.
*/
'######################################################################'

macro Out_pdCIF2_per_xdd(ciffile)                     { Out_pdCIF2_per_xdd(ciffile, "proc", =Get(bkg);) }
macro Out_pdCIF2_per_xdd(ciffile, data_type)          { Out_pdCIF2_per_xdd(ciffile, data_type, =Get(bkg);) }
macro Out_pdCIF2_per_xdd(ciffile, data_type, bkq_eqn) {
/*
If you wish to change the output on a per-xdd basis, use this macro in each xdd.
Otherwise, use Out_pdCIF()
*/
	Out_pdCIF2_xdd_setup
	if Obj_There(str) { for strs { Out_pdCIF2_STR(ciffile, CIF_PHASE_ID, CIF_ID) } }
	Out_pdCIF2_diffraction_data(ciffile, data_type, bkq_eqn, CIF_PHASE_ID, CIF_ID)
}


macro Out_pdCIF(ciffile)                     { Out_pdCIF(ciffile, "proc", =Get(bkg);) }
macro Out_pdCIF(ciffile, data_type)          { Out_pdCIF(ciffile, data_type, =Get(bkg);) }
macro Out_pdCIF(ciffile, data_type, bkq_eqn) { Out_pdCIF(ciffile, data_type, bkq_eqn, , ) }
macro Out_pdCIF(ciffile, data_type, bkq_eqn, n, m) {
/*
This is the macro to use if the structures in each xdd are independent of each other
ie, they are not being co-refined on multiple datasets

Some examples on how to use the Out_pdCIF macro:

-----------------------------------------------
# 1: single pattern, many strs
xdd
	...
	str
		local CIF_PHASE_ID 1
		local !CIF_Z = 2;
		...
	str
		local CIF_PHASE_ID 2
		...
	str
		local CIF_PHASE_ID 3
		...
macro CIF_OUTPUT_FILE { "filename.cif" }
Out_pdCIF(CIF_OUTPUT_FILE)
-----------------------------------------------
-----------------------------------------------
# 2: many patterns, many strs
xdd
	local CIF_ID 1
	local !CIF_SCAN_METHOD = "fixed";
	local CIF_PRES 100
	...
	str
		local CIF_PHASE_ID 1
		...
xdd
	local CIF_ID 2
	local !CIF_SCAN_METHOD = "cont";
	local CIF_PRES 400
	...
	str
		local CIF_PHASE_ID 1
		...
	str
		local CIF_PHASE_ID 2
		local !CIF_Z 6
		...
	str
		local CIF_PHASE_ID 3
		...
...
Out_pdCIF("filename.cif", meas)

-----------------------------------------------
-----------------------------------------------
# 3: sequential refinement, multiple strs

macro CIF_OUTPUT_FILE { "filename.cif" }
num_runs 30
#list File_Name Temperature {
file_01.xye	30
file_02.xye	45
...
}
prm CIF_ID = Run_Number;
prm CIF_TEMP = Temperature(Run_Number);
prm CIF_SCAN_METHOD = "fixed";
xdd
	...
	str
		local CIF_PHASE_ID 1
		local !CIF_Z = 6;
		...
	str
		local CIF_PHASE_ID 2
		local !CIF_Z = 8;
		...
	str
		local CIF_PHASE_ID 3
		...
Out_pdCIF(CIF_OUTPUT_FILE, meas)
-----------------------------------------------
*/
	for xdds n_to_m(n,m) {
		Out_pdCIF2_per_xdd(ciffile, data_type, bkq_eqn)
	}
}

macro Out_pdCIF2_multi(ciffile)                     { Out_pdCIF2_multi(ciffile, "proc", =Get(bkg);) }
macro Out_pdCIF2_multi(ciffile, data_type)          { Out_pdCIF2_multi(ciffile, data_type, =Get(bkg);) }
macro Out_pdCIF2_multi(ciffile, data_type, bkg_eqn) { Out_pdCIF2_multi(ciffile, data_type, bkg_eqn, 1, ) }
macro Out_pdCIF2_multi(ciffile, data_type, bkg_eqn, n, m) {
/*
This is the macro to use if the structures in each xdd are dependent of each other
ie, all structures are being co-refined on all xdds and all structures are present
in all xdds

eg multi-bank TOF data.

Some examples on how to use the Out_pdCIF2_multi macro:

-----------------------------------------------
# 1: multiple patterns, multiple strs
prm CIF_ID 1
prm CIF_TEMP 5
xdd 'eg bank 1 of a tof dataset'
	...
	str
		local CIF_PHASE_ID 1
		...
	str
		local CIF_PHASE_ID 2
		...
	str
		local CIF_PHASE_ID 3
		...
xdd 'eg bank 2 of a tof dataset'
	...
	str
		local CIF_PHASE_ID 1
		...
	str
		local CIF_PHASE_ID 2
		local !CIF_Z 6
		...
	str
		local CIF_PHASE_ID 3
		...
...
Out_pdCIF2_multi("filename.cif", proc)

-----------------------------------------------
-----------------------------------------------
# 2: sequential refinement, multiple strs,

macro CIF_OUTPUT_FILE { "filename.cif" }
num_runs 30
#list File_Name_b1 File_Name_b2 File_Name_b3 Temperature {
file_b1_01.xye	file_b2_01.xye	file_b3_01.xye	30
file_b1_02.xye	file_b2_02.xye	file_b3_02.xye	35
...
}
prm CIF_ID = Run_Number;
prm CIF_TEMP = Temperature(Run_Number);
xdd File_Name_b1(Run_Number)
	...
	str
		...
	str
		...
xdd File_Name_b2(Run_Number)
	...
	str
		...
	str
		...
xdd File_Name_b3(Run_Number)
	...
	str
		...
	str
		...
for xdds {
	local CIF_SCAN_METHOD = "cont";
	for strs 1 to 1 {
		local CIF_PHASE_ID 1
		local CIF_Z = 6;
	}
	for strs 2 to 2 {
		local CIF_PHASE_ID 2
		local CIF_Z = 8;
	}
}
Out_pdCIF2_multi(CIF_OUTPUT_FILE, meas)
-----------------------------------------------
*/
	for xdds n_to_m(n,m) {
		Out_pdCIF2_xdd_setup
		local CIF_MULTI_DIFFRACTOGRAM_ID  1
	}
	Out_CIF2_newfile(ciffile)
	for xdds n to n { if Obj_There(str) { for strs { Out_pdCIF2_STR(ciffile, CIF_PHASE_ID, CIF_ID, n, m) } } }
	for xdds n_to_m(n,m) { Out_pdCIF2_diffraction_data(ciffile, data_type, bkg_eqn, CIF_PHASE_ID, CIF_ID) }
}
#endif
