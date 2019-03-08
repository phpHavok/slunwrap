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

    [root]# sinfo -h -N -t down -o %N | ./slunwrap
    cnode005
    cnode014
    cnode030
    cnode031
    cnode033
    cnode040
    cnode059
    cnode075
    cnode079
    cnode084
    cnode095
    cnode101
    cnode105
    cnode111
    cnode112
    cnode113
    cnode116
    cnode124
    cnode126
    cnode132
    cnode141
    cnode150
    cnode164
    cnode173
    cnode174
    cnode187
    fnode000
    fnode002
    gnode005
    gnode006
    gnode007
    gnode008
    gnode009
    gnode010
    gnode011
    gnode012
    gnode013
    gnode014
    gnode015
    gnode016
    gnode017
    gnode018
    gnode019
    gnode020
    gnode021
    gnode022
    gnode023
    gnode024
    gnode025
    gnode026
    gnode027
    gnode028

## Compilation

Slunwrap is written in strict ANSI C, so it should compile with no issues on
most GNU/Linux systems as long as `gcc` and `make` is installed. Just type
`make` to build Slunwrap. Manually copy it to `/usr/local/bin` or some place in
your `$PATH` if you want.
