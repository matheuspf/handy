/** @file

    @brief This is a very simple header that allows the creation of classes with (almost) python-like initialization.

    @details It is only supported in C++17, with the aid of the @link http://en.cppreference.com/w/cpp/utility/any 
             std::any @endlink class. Also, it incurs a small runtime overhead, and may be prone to type casting 
             errors, but the result is simply beautiful. Ex:

    @snippet Helpers/HandyParamsExample.cpp Handy Params snippet
**/


#ifndef HANDY_HELPERS_PARAMS_H
#define HANDY_HELPERS_PARAMS_H

#include "Helpers.h"

#include <map>
#include <string>
#include <any>

/** @defgroup HandyParamsGroup Class initialization
    @copydoc HandyParams.h
**/

//@{
/** @name
    @brief The code generators for each parameter
    @note It supports up to 9 parameters, but you can easily copy-paste and create more
**/
//@{
#define HANDY_ATT_PARAMS1(p1, ...) auto it1 = mp.find(#p1);  if(it1 != mp.end()) p1 = std::any_cast<decltype(p1)>(it1->second);
#define HANDY_ATT_PARAMS2(p2, ...) auto it2 = mp.find(#p2);  if(it2 != mp.end()) p2 = std::any_cast<decltype(p2)>(it2->second); HANDY_ATT_PARAMS1(__VA_ARGS__)
#define HANDY_ATT_PARAMS3(p3, ...) auto it3 = mp.find(#p3);  if(it3 != mp.end()) p3 = std::any_cast<decltype(p3)>(it3->second); HANDY_ATT_PARAMS2(__VA_ARGS__)
#define HANDY_ATT_PARAMS4(p4, ...) auto it4 = mp.find(#p4);  if(it4 != mp.end()) p4 = std::any_cast<decltype(p4)>(it4->second); HANDY_ATT_PARAMS3(__VA_ARGS__)
#define HANDY_ATT_PARAMS5(p5, ...) auto it5 = mp.find(#p5);  if(it5 != mp.end()) p5 = std::any_cast<decltype(p5)>(it5->second); HANDY_ATT_PARAMS4(__VA_ARGS__)
#define HANDY_ATT_PARAMS6(p6, ...) auto it6 = mp.find(#p6);  if(it6 != mp.end()) p6 = std::any_cast<decltype(p6)>(it6->second); HANDY_ATT_PARAMS5(__VA_ARGS__)
#define HANDY_ATT_PARAMS7(p7, ...) auto it7 = mp.find(#p7);  if(it7 != mp.end()) p7 = std::any_cast<decltype(p7)>(it7->second); HANDY_ATT_PARAMS6(__VA_ARGS__)
#define HANDY_ATT_PARAMS8(p8, ...) auto it8 = mp.find(#p8);  if(it8 != mp.end()) p8 = std::any_cast<decltype(p8)>(it8->second); HANDY_ATT_PARAMS7(__VA_ARGS__)
#define HANDY_ATT_PARAMS9(p9, ...) auto it9 = mp.find(#p9);  if(it9 != mp.end()) p9 = std::any_cast<decltype(p9)>(it9->second); HANDY_ATT_PARAMS8(__VA_ARGS__)
//@}


/** @brief Simply call this guy at the public section of your class, passing the class name as first argument
           and a sequence of the arguments you want to be capable of initialization. For examples of usage,
           see the 'examples' folder.
  
    @warning The types passed by parameter must be exactly equal to the ones in the class. That is,
             if theres a @c float in your class, and you pass a @c double, std::any will throw an exception.
*/
#define HANDY_PARAMS(CLASS, ...)	\
CLASS(std::map<std::string, std::any> mp) { APPLY_N(HANDY_ATT_PARAMS, __VA_ARGS__) }

//@}



#endif // HANDY_HELPERS_PARAMS_H