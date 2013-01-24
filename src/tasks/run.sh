#/bin/bash
make
./lu_tiled.exec test/mat_1024.in test/mat_1024.out 8
python ../diffpy/diff.py test/mat_1024.out test/real_mat_1024.out
