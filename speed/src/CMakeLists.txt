# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Build the cppad_ipopt/src library
# Inherit build type from ../CMakeList.txt

#  add_library(<name> [STATIC | SHARED | MODULE] [EXCLUDE_FROM_ALL]
#	source1 source2 ... sourceN)
# )
# Make libspeed_src a static library because this is just for testing
# and is not installed (do not have to worry about library search path).
SET(source_list
	link_det_lu.cpp
	link_det_minor.cpp
	link_mat_mul.cpp
	link_ode.cpp
	link_poly.cpp
	link_sparse_hessian.cpp
	link_sparse_jacobian.cpp
	microsoft_timer.cpp
)
# 2DO: fix problem with the test
#	./speed_cppad sparse_hessian 123 colpack
# when speed_cppad is compiled for debugging.
#
set_compile_flags( speed_src "${cppad_debug_which}" "${source_list}" )
#
ADD_LIBRARY(speed_src STATIC EXCLUDE_FROM_ALL ${source_list} )

