#/bin/bash
make
./lu_tiled.exec test/mat_64.in test/mat_64.out 8
python ../diffpy/diff.py test/mat_64.out test/real_mat_64.out
