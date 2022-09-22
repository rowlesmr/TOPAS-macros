# Powder CIF Output

The macros below are from Matthew Rowles and should produce full powder cifs in a journal-friendly format. There are recipes for multiple data sets, multiple strs, x-rays and neutrons and magnetic structures. The CIFs can be viewed in 2D and 3D using pdCIFplotter.py giving a proper record of the final fit. More details and refernces to follow. All the information on how to do things is in the macros. As it says, head to the bottom (“Some examples on how to use the Out_pdCIF macro:”) for info.

The whole file needs to be saved as cif.inc in your main topas directory and the line “#include cif.inc” added to your local.inc.

You can read the paper in [J. Appl. Cryst.](https://onlinelibrary.wiley.com/doi/pdf/10.1107/S1600576722003478) or more about the project at the [IUCr powder commission pages](https://www.iucr.org/resources/commissions/powder-diffraction/projects/pdcif). Please cite if you use.

You can download the windows version at from the [IUCr page](https://www.iucr.org/resources/commissions/powder-diffraction/projects/pdcif). It will be slow loading as it has to do a temporary install of all the python gubbins on your computer.

Install the python version with: pip install pdCIFplotter. 

On windows you may need to preintall PyCifRW. Follow the instructions on how to do this [https://pypi.org/project/pdCIFplotter/|here]].

Run the python version with: python -m pdCIFplotter

Update the python version with: pip install -–upgrade pdCIFplotter (2 minus signs in front of upgrade).

More details at the links above. For me the python version works from the powershell but not a cmd prompt.
 
