/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

/**\file
  \brief Metaprogramming support for processing functions of multiple arguments
*/

#ifndef FUNCTION_H
#define FUNCTION_H
#include "classdesc.h"
#include <string>

namespace classdesc
{

  /**
     \namespace classdesc::functional \brief contains code generated by
     functiondb.sh that defines functional attributes.
  */
  namespace functional
  {

    // these classes have either a const static member V, or a type T
    /// \c Arity::V (or ::value) is the number of arguments of
    //function object \a F
    template <class F> struct Arity;
    /// \c Return::T (or ::type) is the return type of \a F
    template <class F> struct Return;
    /// \c is_member_function_ptr::value is true if \a F is a member function pointer
    // note - this basically duplicates std::is_member_function_pointer
    template <class F> struct is_member_function_ptr
    {static const bool value=false;};

    /// \c is_const_method::value is true if F is a pointer to a const method
    template <class F> struct is_const_method
    {static const bool value=false;};

    /// \c is_nonmember_function_ptr::value is true if \a F is an ordinary function pointer
    template <class F> struct is_nonmember_function_ptr
    {static const bool value=false;};
    /// is_member_function_ptr<F>||is_nonmember_function_ptr<F>
    /// note this is semantically different from std::is_function
    template <class F> struct is_function
    {
      static const bool value=is_member_function_ptr<F>::value||
      is_nonmember_function_ptr<F>::value;
    };


    

    /// @{ apply metaprogramming predicate to all arguments of a
    /// functional, and reduce by && or ||
    template <class F, template<class> class Pred, int arity=Arity<F>::value> struct AllArgs;
//    template <class F, template<class> class Pred> struct AnyArg
//    {
//      static const bool value=!AllArgs<F,Not<Pred> >::value;
//    };
   /// @}

    /** \c Arg<F,i> is the type of argument \a i of \a F, i=1..Arity<F> */

    template <class F, int> struct Arg;

    template <class C, class M> class bound_method;

    template <class T> struct Fdummy {Fdummy(int) {} };


    /**
       apply function f to A arguments. Called from \c apply

       Function definitions below left for documentation purposes, but
       commented out to cause hard compilation failures when F is not
       supported (eg has lvalue arguments)

    template <class F, class Args> inline
    typename Return<F>::T 
    apply_nonvoid_fn(F f, const Args& args, Fdummy<F> dum=0);

    template <class F, class Args>  inline
    void apply_void_fn(F f, const Args& args, Fdummy<F> dum=0);
    */

    template <class C, class M> 
    struct Arity<bound_method<C,M> >
    {
      static const int V=Arity<M>::V;
      static const int value=V;
    };

    template <class C, class M> 
    struct Return<bound_method<C,M> >
    {
      typedef typename Return<M>::T T;
      typedef T type;
    };

    template <class C, class M, int i> 
    struct Arg<bound_method<C,M>,i>
    {
      typedef typename Arg<M,i>::T T;
      typedef T type;
    };

#include "functiondb.h"

#if defined(__cplusplus) && __cplusplus>=201103L 
    // for generic function objects
    template <class F> 
    struct Arity
    {
      static const int V=Arity<decltype(&F::operator())>::V;
      static const int value=V;
    };

    template <class F> 
    struct Return
    {
      typedef typename Return<decltype(&F::operator())>::T T;
      typedef T type;
    };

    template <class F, int i> 
    struct Arg
    {
      typedef typename Arg<decltype(&F::operator()),i>::T T;
      typedef T type;
    };
#endif
    
    template <class O, class M>
    bound_method<O,M> bindMethod(O& o, M m)
    {return bound_method<O,M>(o,m);}

    template <class F, class Args>
    typename enable_if< 
      Not<is_void<typename Return<F>::T> >,
      void
    >::T
    apply(typename Return<F>::T* r, F f, const Args& args, dummy<0> d=0)
    {*r=apply_nonvoid_fn(f,args);}
  
    template <class F, class Args>
    typename enable_if<is_void<typename Return<F>::T>, void>::T
    apply(void* r, F f, Args& args, dummy<1> d=0)
    {apply_void_fn(f,args);}
  
    


  }

}
#endif
