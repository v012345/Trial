#ifndef INCLUDED_SEQUENCE_BASE_H
#define INCLUDED_SEQUENCE_BASE_H

namespace Sequence{

class Base{
public:
	virtual ~Base(){} //不必做什么
	virtual Base* update( Base* ) = 0;
};

} //namespace Sequence

#endif