# Slunwrap

Slunwrap (Slurm Unwrapper) is a utility to help facilitate node automation
scripts by unwrapping the compact syntax that Slurm spits out when listing
nodes.  For example, consider the following command which outputs a list of
nodes in the "down" state.

    [root]# sinfo -h -N -t down -o %N
    cnode[005,014,030-031,033,040,059,075,079,084,095,101,105,111-113,116,124,126,132,141,150,164,173-174,187],fnode[000,002],gnode[005-028]

The administrator may want to see these nodes listed one per line with all
formatting removed. That's where Slunwrap comes in. Following is the same
example using Slunwrap.

TODO

## Compilation

TODO
