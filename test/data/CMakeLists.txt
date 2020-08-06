# test/data/CMakeLists.txt

# (c) British Crown Owned Copyright 2020/AWE
#
# This file is part of TIO Browse
# Released under the BSD 3-clause license.
# For more details see license.txt

# Copy test data to build directory
configure_file(ex_colinear_mesh.h5 ex_colinear_mesh.h5 COPYONLY)
configure_file(ex_noncolinear_mesh.h5 ex_noncolinear_mesh.h5 COPYONLY)
configure_file(ex_unstructured_mesh.h5 ex_unstructured_mesh.h5 COPYONLY)
configure_file(ex_vargroup.h5 ex_vargroup.h5 COPYONLY)
configure_file(3d_chunk_v1.h5 3d_chunk_v1.h5 COPYONLY)
configure_file(3d_point_v1.h5 3d_point_v1.h5 COPYONLY)
