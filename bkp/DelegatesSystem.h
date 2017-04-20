#pragma once

#ifndef DELEGATES_SYSTEM_H
#define DELEGATES_SYSTEM_H

#include <vector>
#include <string>
#include <type_traits>
//#include "D3D.h"

#include "map_vector.h"


template<class T> using get_type = typename T::type;

template<class T> using unconst = get_type<std::remove_const<T>>;
template<class T> using unref = get_type<std::remove_reference<T>>;
template<class T> using unconstref = unconst<unref<T>>;


using namespace std;

namespace delegates_system
{


	class VOID_TYPE{};

	class IArguments{ public: virtual ~IArguments() {} };

	template<class A1 = VOID_TYPE, class A2 = VOID_TYPE, class A3 = VOID_TYPE, class A4 = VOID_TYPE>
	class Arguments : public IArguments
	{
	public:

		Arguments() = default;

		Arguments(A1 _arg1, A2 _arg2, A3 _arg3, A4 _arg4) : arg1(_arg1), arg2(_arg2), arg3(_arg3), arg4(_arg4) {}
		Arguments(A1 _arg1, A2 _arg2) : arg1(_arg1), arg2(_arg2) {}
		Arguments(A1 _arg1) : arg1(_arg1) {}

		A1 arg1;
		A2 arg2;
		A3 arg3;
		A4 arg4;

	};

	template<class A1 = VOID_TYPE, class A2 = VOID_TYPE, class A3 = VOID_TYPE, class A4 = VOID_TYPE>
	class PArguments : public IArguments
	{
	public:

//		typedef std::remove_reference< std::remove_const<A1>::type >::type _A1;

		PArguments() = default;

		PArguments(A1* pArg1, A2* pArg2, A3* pArg3, A4* pArg4) : p_arg1(pArg1), p_arg2(pArg2), p_arg3(pArg3), p_arg4(pArg4) {}
		PArguments(A1* pArg1, A2* pArg2, A3* pArg3) : p_arg1(pArg1), p_arg2(pArg2), p_arg3(pArg3) {}
		PArguments(A1* pArg1, A2* pArg2) : p_arg1(pArg1), p_arg2(pArg2) {}
		PArguments(A1* pArg1) : p_arg1(pArg1) {}

		A1* p_arg1;
		A2* p_arg2;
		A3* p_arg3;
		A4* p_arg4;

	};





	class IDelegateData
	{
	public:

		virtual void bind_args(IArguments*) = 0;
		virtual void invoke(IArguments*) = 0; // вызов со списком передаваемых аргументов
		virtual void call() = 0;  // вызов с внешними аргументами, указатели на которые хранятся внутри
	};

	template<class R, class F>
	class DelegateFunctionData : public IDelegateData {};   // общий интерфейс

	// Специализации

	template<class R>
	class DelegateFunctionData<R, R(*)(void)> : public IDelegateData
	{

	public:

		typedef R(*F)(void);

		DelegateFunctionData(F pF) : f(pF) {}

		void bind_args(IArguments*) override
		{

		}

		void invoke(IArguments* i_args) override
		{
			f();
		}

		void call() override
		{
			f();
		}

	private:

		F f;

	};



	template<class R, class A1>
	class DelegateFunctionData<R, R(*)(A1)> : public IDelegateData
	{

	public:


		typedef unconstref<A1> _A1;


		typedef R(*F)(A1);


		DelegateFunctionData(F pF) : f(pF) {}

		DelegateFunctionData(F pF, _A1* p_arg1) : f(pF), mp_arg1(p_arg1) {}


		void bind_args(IArguments* i_args) override
		{
			mp_arg1 = dynamic_cast<PArguments<_A1>*>(i_args)->p_arg1;
		}


		void invoke(IArguments* i_args) override
		{
			f(dynamic_cast<Arguments<_A1>*>(i_args)->arg1);
		}

		void call() override
		{
			f(*mp_arg1);
		}

	private:

		F f;

		_A1* mp_arg1;

	};


	template<class R, class A1, class A2>
	class DelegateFunctionData<R, R(*)(A1, A2)> : public IDelegateData
	{

	public:

		typedef unconstref<A1> _A1;
		typedef unconstref<A2> _A2;


		typedef R(*F)(A1, A2);


		DelegateFunctionData(F pF) : f(pF) {}

		DelegateFunctionData(F pF, _A1* p_arg1, _A2* p_arg2) : f(pF), mp_arg1(p_arg1), mp_arg2(p_arg2) {}


		void bind_args(IArguments* i_args) override
		{
			PArguments<_A1, _A2>* args = dynamic_cast<PArguments<_A1, _A2>*>(i_args);

			mp_arg1 = args->p_arg1;
			mp_arg2 = args->p_arg2;
		}


		void invoke(IArguments* i_args) override
		{
			Arguments<_A1, _A2>* p_args = dynamic_cast<Arguments<_A1, _A2>*>(i_args);

			f(p_args->arg1, p_args->arg2);
		}

		void call() override
		{
			f(*mp_arg1, *mp_arg2);
		}

	private:

		F f;

		_A1* mp_arg1;
		_A2* mp_arg2;

	};



	template<class R, class A1, class A2, class A3>
	class DelegateFunctionData<R, R(*)(A1, A2, A3)> : public IDelegateData
	{

	public:


		typedef unconstref<A1> _A1;
		typedef unconstref<A2> _A2;
		typedef unconstref<A3> _A3;


		typedef R(*F)(A1, A2, A3);


		DelegateFunctionData(F pF) : f(pF) {}

		DelegateFunctionData(F pF, _A1* p_arg1, _A2* p_arg2, _A3* p_arg3) : f(pF), mp_arg1(p_arg1), mp_arg2(p_arg2), mp_arg3(p_arg3) {}


		void bind_args(IArguments* i_args) override
		{
			PArguments<_A1, _A2, _A3>* args = dynamic_cast<PArguments<_A1, _A2, _A3>*>(i_args);

			mp_arg1 = args->p_arg1;
			mp_arg2 = args->p_arg2;
			mp_arg3 = args->p_arg3;
		}


		void invoke(IArguments* i_args) override
		{
			Arguments<_A1, _A2, _A3>* p_args = dynamic_cast<Arguments<_A1, _A2, _A3>*>(i_args);

			f(p_args->arg1, p_args->arg2, p_args->arg3);
		}

		void call() override
		{
			f(*mp_arg1, *mp_arg2, *mp_arg3);
		}




	private:

		F f;

		_A1* mp_arg1;
		_A2* mp_arg2;
		_A3* mp_arg3;

	};



	template<class R, class A1, class A2, class A3, class A4>
	class DelegateFunctionData<R, R(*)(A1, A2, A3, A4)> : public IDelegateData
	{

	public:


		typedef unconstref<A1> _A1;
		typedef unconstref<A2> _A2;
		typedef unconstref<A3> _A3;
		typedef unconstref<A4> _A4;


		typedef R(*F)(A1, A2, A3, A4);


		DelegateFunctionData(F pF) : f(pF) {}

		DelegateFunctionData(F pF, _A1* p_arg1, _A2* p_arg2, _A3* p_arg3, _A4* p_arg4)
			: f(pF), mp_arg1(p_arg1), mp_arg2(p_arg2), mp_arg3(p_arg3), mp_arg4(p_arg4) {}


		void bind_args(IArguments* i_args) override
		{
			PArguments<_A1, _A2, _A3, _A4>* args = dynamic_cast<PArguments<_A1, _A2, _A3, _A4>*>(i_args);

			mp_arg1 = args->p_arg1;
			mp_arg2 = args->p_arg2;
			mp_arg3 = args->p_arg3;
			mp_arg4 = args->p_arg4;
		}

		void invoke(IArguments* i_args) override
		{
			Arguments<_A1, _A2, _A3, _A4>* p_args = dynamic_cast<Arguments<_A1, _A2, _A3, _A4>*>(i_args);

			f(p_args->arg1, p_args->arg2, p_args->arg3, p_args->arg4);
		}

		void call() override
		{
			f(*mp_arg1, *mp_arg2, *mp_arg3, *mp_arg4);
		}




	private:

		F f;

		_A1* mp_arg1;
		_A2* mp_arg2;
		_A3* mp_arg3;
		_A4* mp_arg4;

	};






	template<class R, class O, class M>
	class DelegateMethodData : public IDelegateData {};    // общий интерфейс


	// Специализации

	template<class R, class O>
	class DelegateMethodData<R, O, R(O::*)(void)> : public IDelegateData
	{
	public:

		typedef R(O::*M)(void);

		DelegateMethodData(O* obj, M method) : m_obj(obj), m_method(method) {}


		void bind_args(IArguments*) override
		{

		}


		void invoke(IArguments* i_args) override
		{
			(m_obj->*m_method)();
		}

		void call()	override
		{
			(m_obj->*m_method)();
		}

	private:

		O* m_obj;
		M  m_method;

	};


	template<class R, class O, class A1>
	class DelegateMethodData<R, O, R(O::*)(A1)> : public IDelegateData
	{
	public:


		typedef unconstref<A1> _A1;


		typedef R(O::*M)(A1);

		DelegateMethodData(O* obj, M method) : m_obj(obj), m_method(method) {}

		DelegateMethodData(O* obj, M method, _A1* arg1) : m_obj(obj), m_method(method), mp_arg1(arg1) {}


		void bind_args(IArguments* i_args) override
		{
			mp_arg1 = dynamic_cast<PArguments<_A1>*>(i_args)->p_arg1;
		}


		void invoke(IArguments* i_args) override
		{
			//Arguments<_A1>* p_args = dynamic_cast<Arguments<_A1>*>(i_args);
			Arguments<_A1>* p_args = static_cast<Arguments<_A1>*>(i_args);
			
			(m_obj->*m_method)(p_args->arg1);
			//(m_obj->*m_method)(dynamic_cast<Arguments<_A1>*>(i_args)->arg1);
		}

		void call() override
		{
			(m_obj->*m_method)(*mp_arg1);
		}

	private:

		O* m_obj;
		M  m_method;

		_A1* mp_arg1;

	};


	template<class R, class O, class A1, class A2>
	class DelegateMethodData<R, O, R(O::*)(A1, A2)> : public IDelegateData
	{
	public:

		typedef unconstref<A1> _A1;
		typedef unconstref<A2> _A2;



		typedef R(O::*M)(A1, A2);



		DelegateMethodData(O* obj, M method) : m_obj(obj), m_method(method) {}

		DelegateMethodData(O* obj, M method, _A1* arg1, _A2* arg2) : m_obj(obj), m_method(method), mp_arg1(arg1), mp_arg2(arg2) {}


		void bind_args(IArguments* i_args) override
		{
			PArguments<_A1, _A2>* args = dynamic_cast<PArguments<_A1, _A2>*>(i_args);

			mp_arg1 = args->p_arg1;
			mp_arg2 = args->p_arg2;
		}


		void invoke(IArguments* i_args) override
		{
			Arguments<_A1, _A2>* args = dynamic_cast<Arguments<_A1, _A2>*>(i_args);

			(m_obj->*m_method)(args->arg1, args->arg2);
		}

		void call() override
		{
			(m_obj->*m_method)(*mp_arg1, *mp_arg2);
		}


	private:

		O* m_obj;
		M  m_method;

		_A1* mp_arg1;
		_A2* mp_arg2;

	};


	template<class R, class O, class A1, class A2, class A3>
	class DelegateMethodData<R, O, R(O::*)(A1, A2, A3)> : public IDelegateData
	{
	public:

		typedef unconstref<A1> _A1;
		typedef unconstref<A2> _A2;
		typedef unconstref<A3> _A3;
		
		


		typedef R(O::*M)(A1, A2, A3);

		DelegateMethodData(O* obj, M method) : m_obj(obj), m_method(method) {}

		DelegateMethodData(O* obj, M method, _A1* arg1, _A2* arg2, _A3* arg3)
		: m_obj(obj), m_method(method), mp_arg1(arg1), mp_arg2(arg2), mp_arg3(arg3) {}


		void bind_args(IArguments* i_args) override
		{
			PArguments<_A1, _A2, _A3>* args = dynamic_cast<PArguments<_A1, _A2, _A3>*>(i_args);

			mp_arg1 = args->p_arg1;
			mp_arg2 = args->p_arg2;
			mp_arg3 = args->p_arg3;
		}


		void invoke(IArguments* i_args) override
		{
			Arguments<_A1, _A2, _A3>* args = dynamic_cast<Arguments<_A1, _A2, _A3>*>(i_args);

			(m_obj->*m_method)(args->arg1, args->arg2, args->arg3);
		}

		void call() override
		{
			(m_obj->*m_method)(*mp_arg1, *mp_arg2, *mp_arg3);
		}


	private:

		O* m_obj;
		M  m_method;

		_A1* mp_arg1;
		_A2* mp_arg2;
		_A3* mp_arg3;

	};


	template<class R, class O, class A1, class A2, class A3, class A4>
	class DelegateMethodData<R, O, R(O::*)(A1, A2, A3, A4)> : public IDelegateData
	{
	public:

		typedef unconstref<A1> _A1;
		typedef unconstref<A2> _A2;
		typedef unconstref<A3> _A3;
		typedef unconstref<A4> _A4;


		typedef R(O::*M)(A1, A2, A3, A4);



		DelegateMethodData(O* obj, M method) : m_obj(obj), m_method(method) {}

		DelegateMethodData(O* obj, M method, _A1* arg1, _A2* arg2, _A3* arg3, _A4* arg4)
			: m_obj(obj), m_method(method), mp_arg1(arg1), mp_arg2(arg2), mp_arg3(arg3), mp_arg4(arg4) {}


		void bind_args(IArguments* i_args) override
		{
			PArguments<_A1, _A2, _A3, _A4>* args = dynamic_cast<PArguments<_A1, _A2, _A3, _A4>*>(i_args);

			mp_arg1 = args->p_arg1;
			mp_arg2 = args->p_arg2;
			mp_arg3 = args->p_arg3;
			mp_arg4 = args->p_arg4;
		}


		void invoke(IArguments* i_args) override
		{
			Arguments<_A1, _A2, _A3, _A4>* args = dynamic_cast<Arguments<_A1, _A2, _A3, _A4>*>(i_args);

			(m_obj->*m_method)(args->arg1, args->arg2, args->arg3, args->arg4);
		}

		void call() override
		{
			(m_obj->*m_method)(*mp_arg1, *mp_arg2, *mp_arg3, *mp_arg4);
		}


	private:

		O* m_obj;
		M  m_method;

		_A1* mp_arg1;
		_A2* mp_arg2;
		_A3* mp_arg3;
		_A4* mp_arg4;

	};



	class Delegate
	{
	public:

		Delegate() = default;

		Delegate(const std::string& Name) : name(Name) {}
					
		template<class R, class F> static
		Delegate Create(F f)
		{
			Delegate d;

			d.bind_fn<R>(f);

			return d;
		}

		template<class R, class O, class M> static
		Delegate Create(O* obj, M method)
		{
			Delegate d;

			d.bind_method<R>(obj, method);

			return d;
		}


	public:
		

		// bind() для функций

		// Для 0 аргументов
		template<class R, class F>
		void bind_fn(F f)
		{
			m_data = new DelegateFunctionData<R, F>(f);
		}


		// Для 1 аргумента
		template<class R, class F, class A1 = VOID_TYPE>
		void bind_fn(F f, A1* arg1)
		{
			m_data = new DelegateFunctionData<R, F>(f, arg1);
		}

		// Для 2 аргументов
		template<class R, class F, class A1 = VOID_TYPE, class A2 = VOID_TYPE>
		void bind_fn(F f, A1* arg1, A2* arg2)
		{
			m_data = new DelegateFunctionData<R, F>(f, arg1, arg2);
		}

		// Для 3 аргументов
		template<class R, class F, class A1 = VOID_TYPE, class A2 = VOID_TYPE, class A3 = VOID_TYPE>
		void bind_fn(F f, A1* arg1, A2* arg2, A3* arg3)
		{
			m_data = new DelegateFunctionData<R, F>(f, arg1, arg2, arg3);
		}

		// Для 4 аргументов
		template<class R, class F, class A1 = VOID_TYPE, class A2 = VOID_TYPE, class A3 = VOID_TYPE, class A4 = VOID_TYPE>
		void bind_fn(F f, A1* arg1, A2* arg2, A3* arg3, A4* arg4)
		{
			m_data = new DelegateFunctionData<R, F>(f, arg1, arg2, arg3, arg4);
		}


		// bind() для методов

		// Для 0 аргументов
		//template<class R, class O, class M, class A1 = VOID_TYPE, class A2 = VOID_TYPE, class A3 = VOID_TYPE, class A4 = VOID_TYPE>
		template<class R, class O, class M>
		void bind_method(O* obj, M method)
		{
			m_data = new DelegateMethodData<R, O, M>(obj, method);
		}

		// Для 1 аргумента
		template<class R, class O, class M, class A1 = VOID_TYPE, class A2 = VOID_TYPE, class A3 = VOID_TYPE, class A4 = VOID_TYPE>
		void bind_method(O* obj, M method, A1* arg1)
		{
			m_data = new DelegateMethodData<R, O, M>(obj, method, arg1);
		}

		// Для 2 аргументов
		template<class R, class O, class M, class A1 = VOID_TYPE, class A2 = VOID_TYPE, class A3 = VOID_TYPE, class A4 = VOID_TYPE>
		void bind_method(O* obj, M method, A1* arg1, A2* arg2)
		{
			m_data = new DelegateMethodData<R, O, M>(obj, method, arg1, arg2);
		}
		
		// Для 3 аргументов
		template<class R, class O, class M, class A1 = VOID_TYPE, class A2 = VOID_TYPE, class A3 = VOID_TYPE, class A4 = VOID_TYPE>
		void bind_method(O* obj, M method, unconstref<A1>* arg1, unconstref<A2>* arg2, unconstref<A3>* arg3)
		{
			m_data = new DelegateMethodData<R, O, M>(obj, method, arg1, arg2, arg3);
		}

		// Для 4 аргументов
		template<class R, class O, class M, class A1 = VOID_TYPE, class A2 = VOID_TYPE, class A3 = VOID_TYPE, class A4 = VOID_TYPE>
		void bind_method(O* obj, M method, A1* arg1, A2* arg2, A3* arg3, A4* arg4)
		{
			m_data = new DelegateMethodData<R, O, M>(obj, method, arg1, arg2, arg3, arg4);
		}



		// bind() для аргументов

		// для 1 аргумента
		template<class A1 = VOID_TYPE, class A2 = VOID_TYPE, class A3 = VOID_TYPE, class A4 = VOID_TYPE>
		void bind_args(A1* arg1)
		{
			m_data->bind_args(&PArguments<A1>(arg1));
		}

		// для 2 аргументов
		template<class A1, class A2>
		void bind_args(A1* arg1, A2* arg2)
		{
			m_data->bind_args(&PArguments<A1, A2>(arg1, arg2));
		}


		// для 3 аргументов
		template<class A1, class A2, class A3>
		void bind_args(A1* arg1, A2* arg2, A3* arg3)
		{
			m_data->bind_args(&PArguments<A1, A2, A3>(arg1, arg2, arg3));
		}

		// для 4 аргументов
		template<class A1, class A2, class A3, class A4>
		void bind_args(A1* arg1, A2* arg2, A3* arg3, A4* arg4)
		{
			m_data->bind_args(&PArguments<A1, A2, A3, A4>(arg1, arg2, arg3, arg4));
		}




		void call() const
		{
			m_data->call();
		}


		void operator()() const
		{
			m_data->invoke(&Arguments<>());
		}

		template<class A1>
		void operator()(A1 const & arg1)
		{
			m_data->invoke(&Arguments<A1>(arg1));
		}

		template<class A1, class A2>
		void operator()(A1 const & arg1, A2 const & arg2)
		{
			m_data->invoke(&Arguments<A1, A2>(arg1, arg2));
		}

		template<class A1, class A2, class A3>
		void operator()(A1 const & arg1, A2 const & arg2, A3 const & arg3)
		{
			m_data->invoke(&Arguments<A1, A2, A3>(arg1, arg2, arg3));
		}

		template<class A1, class A2, class A3, class A4>
		void operator()(A1 const & arg1, A2 const & arg2, A3 const & arg3, A4 const & arg4)
		{
			m_data->invoke(&Arguments<A1, A2, A3, A4>(arg1, arg2, arg3, arg4));
		}


	private:

		std::string name;

		IDelegateData* m_data;

	};



	class DelegatesSystem
	{
	public:

		DelegatesSystem()
		{
			delegates.SetKeys(&delegates_names);
		}
		

	public:

		void add(const Delegate& d, std::string const & name)
		{
			delegates.push_back(d);
			
			delegates_names.push_back(name);
		}

		void add(Delegate&& d, std::string const & name)
		{
			delegates.push_back(std::move(d));

			delegates_names.push_back(name);
		}

		void launch()
		{
			for (const auto& d : delegates)
				d.call();
		}		

	public:

		Delegate& operator[](std::string const & name)
		{
			return delegates[name];
		}

		Delegate& operator[](uint const & idx)
		{
			return delegates[idx];
		}


	private:

		std::vector<std::string> delegates_names;

		typedef map_vector<std::string, Delegate>  DelegatesCollection;

		DelegatesCollection delegates;
		

	};


} // namespace delegates_system


#endif




