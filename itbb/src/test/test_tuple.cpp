/*
    Copyright 2005-2013 Intel Corporation.  All Rights Reserved.

    This file is part of Threading Building Blocks.

    Threading Building Blocks is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    version 2 as published by the Free Software Foundation.

    Threading Building Blocks is distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Threading Building Blocks; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    As a special exception, you may use this file as part of a free software
    library without restriction.  Specifically, if other files instantiate
    templates or use macros or inline functions from this file, or you compile
    this file and link it with other files to produce an executable, this
    file does not by itself cause the resulting executable to be covered by
    the GNU General Public License.  This exception does not however
    invalidate any other reasons why the executable file might be covered by
    the GNU General Public License.
*/

// tbb::flow::tuple (implementation used in tbb::flow)
// if <tuple> is available on the compiler/platform, that version should be the
// one tested.

#include "harness.h"
// this test should match that in graph.h, so we test whatever tuple is
// being used by the join_node.
#if __TBB_CPP11_TUPLE_PRESENT
#define __TESTING_STD_TUPLE__ 1
#include <tuple>
using namespace std;
#else
#define __TESTING_STD_TUPLE__ 0
#include "tbb/compat/tuple"
using namespace tbb::flow;
#endif /*!__TBB_CPP11_TUPLE_PRESENT*/
#include <string>
#include <iostream>

class non_trivial {
public:
    non_trivial() {}
    ~non_trivial() {}
    non_trivial(const non_trivial& other) : my_int(other.my_int), my_float(other.my_float) { }
    int get_int() const { return my_int; }
    float get_float() const { return my_float; }
    void set_int(int newval) { my_int = newval; }
    void set_float(float newval) { my_float = newval; }
private:
    int my_int;
    float my_float;
};

template<typename T1, typename T2, typename T3, typename U1, typename U2, typename U3>
void RunOneComparisonTest() {
    typedef tuple<T1,T2,T3> t_tuple;
    typedef tuple<U1,U2,U3> u_tuple;

    ASSERT(t_tuple((T1)1,(T2)1,(T3)1) == u_tuple((U1)1,(U2)1,(U3)1),NULL);
    ASSERT(t_tuple((T1)1,(T2)0,(T3)1) <  u_tuple((U1)1,(U2)1,(U3)1),NULL);
    ASSERT(t_tuple((T1)1,(T2)1,(T3)1) >  u_tuple((U1)1,(U2)1,(U3)0),NULL);
    ASSERT(t_tuple((T1)1,(T2)0,(T3)1) != u_tuple((U1)1,(U2)1,(U3)1),NULL);
    ASSERT(t_tuple((T1)1,(T2)0,(T3)1) <= u_tuple((U1)1,(U2)1,(U3)0),NULL);
    ASSERT(t_tuple((T1)1,(T2)0,(T3)0) <= u_tuple((U1)1,(U2)0,(U3)0),NULL);
    ASSERT(t_tuple((T1)1,(T2)1,(T3)1) >= u_tuple((U1)1,(U2)0,(U3)1),NULL);
    ASSERT(t_tuple((T1)0,(T2)1,(T3)1) >= u_tuple((U1)0,(U2)1,(U3)1),NULL);

    ASSERT(!(t_tuple((T1)2,(T2)1,(T3)1) == u_tuple((U1)1,(U2)1,(U3)1)),NULL);
    ASSERT(!(t_tuple((T1)1,(T2)2,(T3)1) == u_tuple((U1)1,(U2)1,(U3)1)),NULL);
    ASSERT(!(t_tuple((T1)1,(T2)1,(T3)2) == u_tuple((U1)1,(U2)1,(U3)1)),NULL);
    
    ASSERT(!(t_tuple((T1)1,(T2)1,(T3)1) <  u_tuple((U1)1,(U2)1,(U3)1)),NULL);
    ASSERT(!(t_tuple((T1)1,(T2)1,(T3)1) >  u_tuple((U1)1,(U2)1,(U3)1)),NULL);
    ASSERT(!(t_tuple((T1)1,(T2)1,(T3)1) !=  u_tuple((U1)1,(U2)1,(U3)1)),NULL);

    ASSERT(t_tuple((T1)1,(T2)1,(T3)1) <= u_tuple((U1)1,(U2)1,(U3)1),NULL);
    ASSERT(t_tuple((T1)1,(T2)1,(T3)1) >= u_tuple((U1)1,(U2)1,(U3)1),NULL);

}

void RunTests() {

#if __TESTING_STD_TUPLE__
    REMARK("Testing platform tuple\n");
#else
    REMARK("Testing compat/tuple\n");
#endif
    tuple<int> ituple1(3);
    tuple<int> ituple2(5);
    tuple<double> ftuple2(4.1);

    ASSERT(!(ituple1 == ituple2), NULL);
    ASSERT(ituple1 != ituple2, NULL);
    ASSERT(!(ituple1 > ituple2), NULL);
    ASSERT(ituple1 < ituple2, NULL);
    ASSERT(ituple1 <= ituple2, NULL);
    ASSERT(!(ituple1 >= ituple2), NULL);
    ASSERT(ituple1 < ftuple2, NULL);

    typedef tuple<int,double,float> tuple_type1;
    typedef tuple<int,int,int> int_tuple_type;
    typedef tuple<int,non_trivial,int> non_trivial_tuple_type;
    typedef tuple<double,std::string,char> stringy_tuple_type;
    const tuple_type1 tup1(42,3.14159,2.0f);
    int_tuple_type int_tup(4, 5, 6);
    non_trivial_tuple_type nti;
    stringy_tuple_type stv;
    get<1>(stv) = "hello";
    get<2>(stv) = 'x';

    ASSERT(get<0>(stv) == 0.0, NULL);
    ASSERT(get<1>(stv) == "hello", NULL);
    ASSERT(get<2>(stv) == 'x', NULL);

    ASSERT(tuple_size<tuple_type1>::value == 3, NULL);
    ASSERT(get<0>(tup1) == 42, NULL);
    ASSERT(get<1>(tup1) == 3.14159, NULL);
    ASSERT(get<2>(tup1) == 2.0, NULL);

    get<1>(nti).set_float(1.0);
    get<1>(nti).set_int(32);
    ASSERT(get<1>(nti).get_int() == 32, NULL);
    ASSERT(get<1>(nti).get_float() == 1.0, NULL);

    // converting constructor
    tuple<double,double,double> tup2(1,2.0,3.0f);
    tuple<double,double,double> tup3(9,4.0,7.0f);
    ASSERT(tup2 != tup3, NULL);

    ASSERT(tup2 < tup3, NULL);

    // assignment
    tup2 = tup3;
    ASSERT(tup2 == tup3, NULL);

    tup2 = int_tup;
    ASSERT(get<0>(tup2) == 4, NULL);
    ASSERT(get<1>(tup2) == 5, NULL);
    ASSERT(get<2>(tup2) == 6, NULL);

    // increment component of tuple
    get<0>(tup2) += 1;
    ASSERT(get<0>(tup2) == 5, NULL);

    std::pair<int,int> two_pair( 4, 8);
    tuple<int,int> two_pair_tuple;
    two_pair_tuple = two_pair;
    ASSERT(get<0>(two_pair_tuple) == 4, NULL);
    ASSERT(get<1>(two_pair_tuple) == 8, NULL);

    //relational ops
    ASSERT(int_tuple_type(1,1,0) == int_tuple_type(1,1,0),NULL);
    ASSERT(int_tuple_type(1,0,1) <  int_tuple_type(1,1,1),NULL);
    ASSERT(int_tuple_type(1,0,0) >  int_tuple_type(0,1,0),NULL);
    ASSERT(int_tuple_type(0,0,0) != int_tuple_type(1,0,1),NULL);
    ASSERT(int_tuple_type(0,1,0) <= int_tuple_type(0,1,1),NULL);
    ASSERT(int_tuple_type(0,0,1) <= int_tuple_type(0,0,1),NULL);
    ASSERT(int_tuple_type(1,1,1) >= int_tuple_type(1,0,0),NULL);
    ASSERT(int_tuple_type(0,1,1) >= int_tuple_type(0,1,1),NULL);

    typedef tuple<int,float,double,char> mixed_tuple_left;
    typedef tuple<float,int,char,double> mixed_tuple_right;

    ASSERT(mixed_tuple_left(1,1.f,1,1) == mixed_tuple_right(1.f,1,1,1),NULL);
    ASSERT(mixed_tuple_left(1,0.f,1,1) <  mixed_tuple_right(1.f,1,1,1),NULL);
    ASSERT(mixed_tuple_left(1,1.f,1,1) >  mixed_tuple_right(1.f,1,0,1),NULL);
    ASSERT(mixed_tuple_left(1,1.f,1,0) != mixed_tuple_right(1.f,1,1,1),NULL);
    ASSERT(mixed_tuple_left(1,0.f,1,1) <= mixed_tuple_right(1.f,1,0,1),NULL);
    ASSERT(mixed_tuple_left(1,0.f,0,1) <= mixed_tuple_right(1.f,0,0,1),NULL);
    ASSERT(mixed_tuple_left(1,1.f,1,0) >= mixed_tuple_right(1.f,0,1,1),NULL);
    ASSERT(mixed_tuple_left(0,1.f,1,0) >= mixed_tuple_right(0.f,1,1,0),NULL);

    ASSERT(!(mixed_tuple_left(2,1.f,1,1) == mixed_tuple_right(1.f,1,1,1)),NULL);
    ASSERT(!(mixed_tuple_left(1,2.f,1,1) == mixed_tuple_right(1.f,1,1,1)),NULL);
    ASSERT(!(mixed_tuple_left(1,1.f,2,1) == mixed_tuple_right(1.f,1,1,1)),NULL);
    ASSERT(!(mixed_tuple_left(1,1.f,1,2) == mixed_tuple_right(1.f,1,1,1)),NULL);
    
    ASSERT(!(mixed_tuple_left(1,1.f,1,1) <  mixed_tuple_right(1.f,1,1,1)),NULL);
    ASSERT(!(mixed_tuple_left(1,1.f,1,1) >  mixed_tuple_right(1.f,1,1,1)),NULL);
    ASSERT(!(mixed_tuple_left(1,1.f,1,1) !=  mixed_tuple_right(1.f,1,1,1)),NULL);

    ASSERT(mixed_tuple_left(1,1.f,1,1) <= mixed_tuple_right(1.f,1,1,1),NULL);
    ASSERT(mixed_tuple_left(1,1.f,1,1) >= mixed_tuple_right(1.f,1,1,1),NULL);

    RunOneComparisonTest<int,float,char,float,char,int>();
    RunOneComparisonTest<double,float,char,float,double,int>();
    RunOneComparisonTest<int,float,char,short,char,short>();
    RunOneComparisonTest<double,float,short,float,char,int>();


    // the following should result in a syntax error
    // typedef tuple<float,float> mixed_short_tuple;
    // ASSERT(mixed_tuple_left(1,1.f,1,1) != mixed_short_tuple(1.f,1.f),NULL);

}

int TestMain() {
    RunTests();
    return Harness::Done;
}
