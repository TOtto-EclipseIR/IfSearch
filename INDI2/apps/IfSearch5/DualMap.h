/** @file DualMap.h
*	Declaration of NamedEnum class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once
#include <qglobal.h>

#include <QMap>
#include <QtDebug>

#include <DDTcore.h>
	template <class T1, class T2> class DualMap
	{
	public:
		DualMap(void) {}
		void clear(void)
			{ mapT1toT2.clear(), mapT2toT1.clear(); }
		bool isEmpty(void) const
			{ return mapT1toT2.isEmpty() || mapT2toT1.isEmpty(); }
		int size(void) const
			{ return qMin(mapT1toT2.size(), mapT2toT1.size()); }
		bool contains(const T1 & key) const
			{ return mapT1toT2.contains(key); }
		bool contains(const T2 & key) const
			{ return mapT2toT1.contains(key); }
		T2 at(const T1 & key) const
			{	return contains(key) ? mapT1toT2.value(key) : T2(); }
		T1 at(const T2 & key) const
			{	return contains(key) ? mapT2toT1.value(key) : T1(); }
#if 0
		QVector<T1> all(const T2 & key) const
		{
			(void)key;
			QVector<T1> vec;
			vec.reserve(mapT2toT1.size());
			foreach(T1 t1, mapT2toT1)
				vec.append(t1);
			return vec;
		} // all(T2)
#else
        QList<T1> all(const T1 & key) const
        {
            (void)key;
            return mapT1toT2.keys();
        }
        QList<T1> all(const T2 & key) const
        {
            (void)key;
            return mapT2toT1.keys();
        }
#endif


		bool remove(const T1 & key)
		{
			if (contains(key))
			{
				T2 value = at(key);
				mapT1toT2.remove(key);
				if (contains(value))
				{
					mapT2toT1.remove(value);
					return true;
				}
			}
			return false;
		} // remove(T1)

		bool remove(const T2 & key)
		{
			if (contains(key))
			{
				T1 value = at(key);
				mapT2toT1.remove(key);
				if (contains(value))
				{
					mapT1toT2.remove(value);
					return true;
				}
			}
			return false;
		} // remove(T1)

		bool insertUnique(const T1 & t1, const T2 & t2)
		{
			if (contains(t1) || contains(t2))
				return false;
			mapT1toT2.insert(t1, t2);
			mapT2toT1.insert(t2, t1);
			return true;
		}

//		QList<QPair<T1,T2> > contents(void); // TODO

	private:
		QMap<T1,T2> mapT1toT2;
		QMap<T2,T1> mapT2toT1;
	}; // class DualMap
