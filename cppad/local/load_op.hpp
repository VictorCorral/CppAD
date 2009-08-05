/* $Id$ */
# ifndef CPPAD_LOAD_OP_INCLUDED
# define CPPAD_LOAD_OP_INCLUDED
CPPAD_BEGIN_NAMESPACE

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


/*!
\file load_op.hpp
Setting a variable so that it corresponds to current value of a VecAD element.
*/

/*!
Zero order forward mode implementation of op = LdpOp.

\copydetails forward_load_op_0
*/
template <class Base>
inline void forward_load_p_op_0(
	size_t         i_z         ,
	size_t*        arg         , 
	size_t         num_par     ,
	const Base*    parameter   ,
	size_t         nc_taylor   ,
	Base*          taylor      ,
	size_t         nc_combined ,
	const bool*    variable    ,
	const size_t*  combined    )
{	size_t i_vec = arg[1];

	// Because the index is a parameter, this indexing error should be
	// caught and reported to the user at an when the tape is recording.
	CPPAD_ASSERT_UNKNOWN( i_vec < combined[ arg[0] - 1 ] );


	CPPAD_ASSERT_UNKNOWN( variable != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( combined != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( NumArg(LdpOp) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(LdpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( arg[0] + i_vec < nc_combined );

	size_t combined_index = arg[0] + i_vec;
	size_t i_y_x          = combined[ combined_index ];	
	Base* z  = taylor + i_z * nc_taylor;
	if( variable[ combined_index ] )
	{	CPPAD_ASSERT_UNKNOWN( i_y_x < i_z );
		Base* y_x = taylor + i_y_x * nc_taylor;
		arg[2]    = i_y_x;
		z[0]      = y_x[0];
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( i_y_x < num_par );
		Base y_x  = parameter[i_y_x];
		arg[2]    = 0;
		z[0]      = y_x;
	}
}

/*!
Zero order forward mode implementation of op = LdvOp.

\copydetails forward_load_op_0
*/
template <class Base>
inline void forward_load_v_op_0(
	size_t         i_z         ,
	size_t*        arg         , 
	size_t         num_par     ,
	const Base*    parameter   ,
	size_t         nc_taylor   ,
	Base*          taylor      ,
	size_t         nc_combined ,
	const bool*    variable    ,
	const size_t*  combined    )
{
	CPPAD_ASSERT_UNKNOWN( variable != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( combined != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( NumArg(LdvOp) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(LdvOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( arg[1] < i_z );

	size_t i_vec = Integer( taylor[ arg[1] * nc_taylor + 0 ] );
	CPPAD_ASSERT_KNOWN( 
		i_vec < combined[ arg[0] - 1 ] ,
		"VecAD: index during zero order forward sweep is out of range"
	);
	CPPAD_ASSERT_UNKNOWN( arg[0] + i_vec < nc_combined );

	size_t combined_index = arg[0] + i_vec;
	size_t i_y_x          = combined[ combined_index ];	


	Base* z  = taylor + i_z * nc_taylor;
	if( variable[ combined_index ] )
	{	CPPAD_ASSERT_UNKNOWN( i_y_x < i_z );
		Base* y_x = taylor + i_y_x * nc_taylor;
		arg[2]    = i_y_x;
		z[0]      = y_x[0];
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( i_y_x < num_par );
		Base y_x  = parameter[i_y_x];
		arg[2]    = 0;
		z[0]      = y_x;
	}
}

/*!
Forward mode, except for zero order, for op = LdpOp or op = LdvOp

The C++ source code corresponding to this operation is
\verbatim
	z = y[x]
\endverbatim
where y is a VecAD<Base> vector and x is an AD<Base> or Base index. 

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param op
is the code corresponding to this operator; i.e., LdpOp or LdvOp
(only used for error checking).

\param d
is the order of the Taylor coefficient that we are computing.

\param i_z
is the AD variable index corresponding to the variable z.

\param arg
\a arg[2]
If y[x] is a parameter, \a arg[2] is zero 
(which is not a valid variable index).
If y[x] is a variable, 
\a arg[2] is the variable index corresponding to y[x].

\param nc_taylor
number of columns in the matrix containing the Taylor coefficients.

\param taylor
\b Input: if y[x] is a variable, \a taylor[ \a arg[2] * nc_taylor + d ]
is the d-order Taylor coefficient corresponding to y[x].
\n
\b Output: \a taylor[ i_z * nc_taylor + d ]
is the d-order Taylor coefficient for the variable z.

\par Checked Assertions 
\li NumArg(op) == 3
\li NumRes(op) == 1
\li 0 < d < nc_taylor
\li arg[2] < i_z
*/
template <class Base>
inline void forward_load_op(
	OpCode         op          ,
	size_t         d           ,
	size_t         i_z         ,
	const size_t*  arg         , 
	size_t         nc_taylor   ,
	Base*          taylor      )
{

	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(op) == 1 );
	CPPAD_ASSERT_UNKNOWN( d > 0 )
	CPPAD_ASSERT_UNKNOWN( d < nc_taylor );
	CPPAD_ASSERT_UNKNOWN( arg[2] < i_z );

	Base* z      = taylor + i_z * nc_taylor;
	if( arg[2] > 0 )
	{	Base* y_x = taylor + arg[2] * nc_taylor;
		z[d]      = y_x[d];
	}
	else	z[d]      = Base(0);
}

/*!
Reverse mode for op = LdpOp or LdvOp.

The C++ source code corresponding to this operation is
\verbatim
	z = y[x]
\endverbatim
where y is a VecAD<Base> vector and x is an AD<Base> or Base index. 

This routine is given the partial derivatives of a function 
G(z , y[x] , w , u ... )
and it uses them to compute the partial derivatives of 
\verbatim
	H( y[x] , w , u , ... ) = G[ z( y[x] ) , y[x] , w , u , ... ]
\endverbatim

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param op
is the code corresponding to this operator; i.e., LdpOp or LdvOp
(only used for error checking).

\param d
highest order the Taylor coefficient that we are computing the partial
derivative with respect to.

\param i_z
is the AD variable index corresponding to the variable z.

\param arg
\a arg[2]
If y[x] is a parameter, \a arg[2] is zero 
(which is not a valid variable index).
If y[x] is a variable, 
\a arg[2] is the variable index corresponding to y[x].

\param nc_taylor
number of columns in the matrix containing the Taylor coefficients
(not used).

\param taylor
matrix of Taylor coefficients (not used).

\param nc_partial
number of colums in the matrix containing all the partial derivatives
(not used if \a arg[2] is zero).

\param partial
If \a arg[0] is zero, y[x] is a parameter
and no values need to be modified; i.e., \a partial is not used.
Otherwise, y[x] is a variable and:
\n
\n
\a partial [ \a i_z * \a nc_partial + k ] 
for k = 0 , ... , \a d
is the partial derivative of G
with respect to the k-th order Taylor coefficient for z.
\n
\n
If \a arg[0] is not zero,
\a partial [ \a arg[2] * \a nc_partial + k ]
for k = 0 , ... , \a d
is the partial derivative with respect to 
the k-th order Taylor coefficient for x.
On input, it corresponds to the function G,
and on output it corresponds to the the function H. 

\par Checked Assertions 
\li NumArg(op) == 3
\li NumRes(op) == 1
\li d < nc_taylor
\li arg[2] < i_z
*/
template <class Base>
inline void reverse_load_op(
	OpCode         op          ,
	size_t         d           ,
	size_t         i_z         ,
	const size_t*  arg         , 
	size_t         nc_taylor   ,
	const Base*    taylor      ,
	size_t         nc_partial  ,
	Base*          partial     )
{

	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(op) == 1 );
	CPPAD_ASSERT_UNKNOWN( d < nc_taylor );
	CPPAD_ASSERT_UNKNOWN( arg[2] < i_z );

	if( arg[2] > 0 )
	{
		Base* pz   = partial + i_z    * nc_partial;
		Base* py_x = partial + arg[2] * nc_partial;
		size_t j = d + 1;
		while(j--)
			py_x[j]   += pz[j];
	}
}


/*!
Forward mode sparsity operations for LdpOp and LdvOp

\copydetails sparse_load_op
*/
template <class Pack>
inline void forward_sparse_load_op(
	OpCode         op           ,
	size_t         i_z          ,
	const size_t*  arg          , 
	size_t         num_row      ,
	const size_t*  row          ,
	size_t         num_vec      ,
	size_t         nc_sparsity  ,
	Pack*          var_sparsity ,
	Pack*          vec_sparsity )
{
	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(op) == 1 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( arg[0] < num_row );
	size_t i_y = row[ arg[0] - 1 ];
	CPPAD_ASSERT_UNKNOWN( i_y < num_vec );

	Pack* z = var_sparsity + nc_sparsity * i_z;
	Pack* y = vec_sparsity + nc_sparsity * i_y;

	size_t j = nc_sparsity;
	while(j--)
		z[j] = y[j];

	return;
}


/*!
Reverse mode sparsity operations for LdpOp and LdvOp

\copydetails sparse_load_op
*/
template <class Pack>
inline void reverse_sparse_load_op(
	OpCode         op           ,
	size_t         i_z          ,
	const size_t*  arg          , 
	size_t         num_row      ,
	const size_t*  row          ,
	size_t         num_vec      ,
	size_t         nc_sparsity  ,
	Pack*          var_sparsity ,
	Pack*          vec_sparsity )
{
	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(op) == 1 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( arg[0] < num_row );
	size_t i_y = row[ arg[0] - 1 ];
	CPPAD_ASSERT_UNKNOWN( i_y < num_vec );

	Pack* z = var_sparsity + nc_sparsity * i_z;
	Pack* y = vec_sparsity + nc_sparsity * i_y;

	size_t j = nc_sparsity;
	while(j--)
		y[j] |= z[j];

	return;
}



CPPAD_END_NAMESPACE
# endif
