/*****************
the type definition here 
path:include/types/types.h
*******************/

#ifndef __TYPES_H__
#define __TYPES_H__


class  NonCopyable {
public:
	NonCopyable ();
	~NonCopyable ();
protected:
    NonCopyable (const NonCopyable& ) ;
    NonCopyable& operator = (const NonCopyable& ) ;
};


#endif //__TYPES_H__
