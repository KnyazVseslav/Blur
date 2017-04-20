#pragma once

#ifndef MAP_VECTOR_H
#define MAP_VECTOR_H

typedef unsigned int uint;

#include <vector>

template<class Key, class Val>
class map_vector : public std::vector<Val>
{
public:

	Val& operator[](const Key& idx);
	Val& operator[](const uint& idx);

	void SetKeys(std::vector<Key>* keys); // сохранить указатель на вектор ключей, чтобы избежать дублирования данных
	//void SetKeys(std::vector<Key>::iterator keys);

	void add(Val& val);
	void add(Val&& val);


private:

	uint index_of(const std::vector<Key>& key_vector, const Key& search_val);

	std::vector<Key>* keys; // храним указатель на вектор ключей, чтобы избежать дублирования данных

};




// нужно для реализации ассоциативного доступа к вектору (подобно доступу к отображению)
template<class Key, class Val>
uint map_vector<Key, Val>::index_of(const std::vector< sptr_of<Key> >& key_vector, const Key& search_val)
{

	std::vector<Key>::const_iterator it =
		search_n(key_vector.begin(), key_vector.end(), 1, search_val, [&](const auto& it){ return (*it == search_val); });

	return it - key_vector.cbegin();

	//return 0;

}


// Перегрузка оператора индекса для доступа по ассоциативной семантике
template<class Key, class Val>
Val& map_vector<Key, Val>::operator[](const Key& idx)
{
	return std::vector<Val>::operator[](index_of(*keys, idx)); // поиск заданного ключа idx в векторе ключей std::vector<Key>* keys и возвращение по индексу 
	// данного ключа, связанного с idx значением
}


// Перегрузка оператора индекса также и для обычного доступа по индексной семантике, т.к. появление ассоциативной перегрузки скрывало данную
template<class Key, class Val>
Val& map_vector<Key, Val>::operator[](const uint& idx)
{
	return std::vector<Val>::operator[](idx);
}


// сохранить указатель на вектор ключей, чтобы избежать дублирования данных
template<class Key, class Val>
void map_vector<Key, Val>::SetKeys(sptr_of< std::vector< sptr_of<Key> >  > keys)
{
	this->keys = keys;
}


template<class Key, class Val>
void map_vector<Key, Val>::add(Val& val)
{
	push_back(val);
}


template<class Key, class Val>
void map_vector<Key, Val>::add(Val&& val)
{
	push_back(std::move(val));
}






#endif

