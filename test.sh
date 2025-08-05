#!/bin/bash

./minishell << EOF
pwd
cd /tmp
echo \$PWD
echo \$OLDPWD
cd -
export Z=1
export A=2
export M=3
export
unset 1INVALID
unset !bad
exit
EOF
