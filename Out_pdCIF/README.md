# Powder CIF Output

The macros below are from Matthew Rowles and should produce full powder cifs in a journal-friendly format. There are recipes for multiple data sets, multiple strs, x-rays and neutrons and magnetic structures. More details and references to follow. All the information on how to do things is in the macros. As it says, head to the bottom (�Some examples on how to use the Out_pdCIF2 macro:�) for info.

There is duplication of some elements due to the way TOPAS hand;es output. To clean them up, you can manually remove duplicate blocks, or use something like [cifflow](https://github.com/rowlesmr/cifflow) to automate the process.

The whole file needs to be saved as cif.inc in your main topas directory and the line �#include cif.inc� added to your local.inc.
