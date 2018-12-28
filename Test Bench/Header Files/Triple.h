#pragma once

#include <utility>
// Desperate times call for desperate measures :)

template <class T, class T1, class T2>
class Triple {
	public:
		T first()
		{
			return a.first.first;
		}
		T1 second()
		{
			return a.first.second;
		}
		T2 third()
		{
			return a.second;
		}
	protected:
		std::pair<std::pair<T, T1>, T2> a;
};