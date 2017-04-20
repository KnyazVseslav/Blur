
#pragma once

#ifndef SHADER_SYSTEM_H
#define SHADER_SYSTEM_H

//#include "map_vector.h"
#include "DelegatesSystem.h"
#include "map_vector.h"



/*
#include <d3dcompiler.h>

#include "modules\Facilities_LIB\Facilities_LIB.h"

#include "modules\DDSTextureLoader\DDSTextureLoader.h"

//#include "query.h"
#include "modules\dxerr\dxerr.h" // DXTrace()

*/

//#include <iostream>
#include <utility>
#include <algorithm>
#include <string>
#include <vector>
#include <d3d11.h>

/*
#pragma comment (lib, "modules\\DDSTextureLoader\\DDSTextureLoader.lib")
#pragma comment (lib, "modules\\dxerr\\dxerr.lib")
#pragma comment (lib, "modules\\Facilities_LIB\\Facilities.lib") // string_facilities::pchar_to_pwchar(char* str)
#pragma comment (lib, "d3dcompiler.lib") // for employement of D3DCompileFromFile wich will be accessible after unplugging legacy SDK, for now D3DX11CompileFromFile()
*/

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace delegates_system;

namespace effects
{

namespace shader_system
{



//typedef unsigned int uint;


class Shader;


/*
template<class Key, class Val>
class map_vector: public std::vector<Val>
{
public:

	Val& operator[](const Key& idx);
	Val& operator[](const uint& idx);

	void SetKeys(std::vector<Key>* keys); // сохранить указатель на вектор ключей, чтобы избежать дублирования данных
	//void SetKeys(std::vector<Key>::iterator keys);


private:

	uint index_of(const std::vector<Key>& key_vector, const Key& search_val);

	std::vector<Key>* keys; // храним указатель на вектор ключей, чтобы избежать дублирования данных

};
*/

class ShaderSystem;




class Effects
{
public:

	Effects();

	void Init(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext);

	void Add(const std::string& name); // используется когда у всех эффектов одни и те же d3dDevice и d3dDeviceContext
	void Add(const std::string& name, ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext); // если d3dDevice и d3dDeviceContext
																											 // у каждого эффекта свои

	void Add(const std::string& name, const ShaderSystem& effect); // если нужно добавить готовый эффект
	void Add(const std::string& name, const ShaderSystem& effect, 
		               ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext); // если нужно добавить готовый эффект с новыми
																						// d3dDevice и d3dDeviceContext

	uint size(); // количество эффектов в коллекции

	



	ShaderSystem& operator[](const std::string& key);
	ShaderSystem& operator[](const uint& idx);
	

private:

  typedef map_vector<std::string, ShaderSystem> EffectsCollection;

	EffectsCollection effects;	
	


	std::vector<std::string> names;




	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dDeviceContext;

};



enum ShaderTypes{VertexShader, HullShader, DomainShader, GeometryShader, PixelShader, ComputeShader};




class ShaderSystem
{

public:

	ShaderSystem();	
	//{
	//	shaders.SetKeys(&shader_names); // map_vector<>::SetKeys() - передаём указатель на вектор имён шейдеров, вместо копирования;
										// далее контейнер map_vector будет использовать имена шейдеров для доступа к шейдерам по ключу
	//}

	ShaderSystem(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext);
	

	void Init(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext); // использовать этот метод, когда объект будет применять эффекты, поставляемые ShaderSystem
	

	void AddShader(std::string shader_name,				   
				   const ShaderTypes& ShaderType,
				   char* szFileName, 
				   LPCSTR szEntryPoint, 
				   LPCSTR szShaderModel);

	void set();



	void begin();
	void end();


	void AddBeginFunction(delegates_system::Delegate f, std::string const & name);
	void AddEndFunction(delegates_system::Delegate f, std::string const & name);


	delegates_system::DelegatesSystem begin_functions;
	delegates_system::DelegatesSystem end_functions;


	typedef map_vector<std::string, Shader> ShadersCollection;
	
	ShadersCollection shaders; // коллекция шейдеров


	  ID3DBlob* get_VS_byte_code();

	  void set_device(ID3D11Device* d3dDevice);
	  void set_device_context(ID3D11DeviceContext* d3dDeviceContext);


private:

	std::vector<std::string> shader_names;	
	
	ID3DBlob* VS_byte_code;	

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dDeviceContext;	

};

class IConstantBuffer;

template<class T> class ConstantBuffer;





	class ISetter
	{
	public:

		virtual void set_shader() = 0;

		virtual void set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers) = 0;

		virtual void set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV) = 0;

	protected:

		ID3D11DeviceContext* d3dDeviceContext;

	};


	class VertexShaderSetter: public ISetter
	{
	public:
		
		VertexShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11VertexShader* VS);

		void set_shader();

		void set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers);

		void set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV);
	
	private:

		ID3D11VertexShader* VS;

	};


	class HullShaderSetter: public ISetter
	{
	public:

		HullShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11HullShader* HS);
	
		void set_shader();

		void set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers);

		void set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV);
	
	private:

		ID3D11HullShader* HS;

	};


	class DomainShaderSetter: public ISetter
	{
	public:

		DomainShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11DomainShader* DS);
		
		void set_shader();

		void set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers);

		void set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV);
	
	private:

		ID3D11DomainShader* DS;

	};


	class GeometryShaderSetter: public ISetter
	{
	public:

		GeometryShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11GeometryShader* GS);

		void set_shader();
		
		void set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers);

		void set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV);
		
	private:

		ID3D11GeometryShader* GS;

	};
	

	class PixelShaderSetter: public ISetter
	{
	public:

		PixelShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11PixelShader* PS);

		void set_shader();
		
		void set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers);

		void set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV);
		
	private:

		ID3D11PixelShader* PS;

	};


	class ComputeShaderSetter: public ISetter
	{
	public:
		
		ComputeShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11ComputeShader* CS);

		void set_shader();
		
		void set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers);

		void set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV);
		
	
	private:

		ID3D11ComputeShader* CS;

	};



class Texture2D;


class Shader
{

public:

	Shader()
	{
	
	};


	Shader(std::string shader_name,
		   ID3D11Device* d3dDevice,	
		   ID3D11DeviceContext* d3dDeviceContext, 
		   const ShaderTypes& ShaderType,
		   WCHAR* szFileName, 
		   LPCSTR szEntryPoint, 
		   LPCSTR szShaderModel);


	Shader(const Shader& rhs);

	
	std::string& get_name();

	template<class T> void AddConstantBuffer(const std::string& buffer_name,
										     const uint& StartSlot,
											 const T* content = NULL,
										     const uint& NumBuffers = 1,
										     D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC, 
										     UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE, 
										     UINT MiscFlags = 0);

	
	void AddTexture2D(char* file_name, const uint& StartSlot, const uint& NumViews = 1);

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel);

	void create(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel); // происходит компиляция шейдера и выбор установщика на основе 
																			   // типа шейдера
	//void create(

	void set();

	void set_Texture2D(ID3D11ShaderResourceView* const * SRV, const uint& StartSlot, const uint& NumViews);



	typedef map_vector<std::string, IConstantBuffer*> ConstantBuffersCollection; // интерфейс IConstantBuffer нужен для того, чтобы можно было
																		    // создать коллекцию КБ, не указывая параметр шаблона ConstantBuffer<T>,
																			// т.к. мы не знаем заранее какой будет тип содержимого КБ
	
		 ConstantBuffersCollection constant_buffers; // коллекция КБ


	typedef map_vector<std::string, Texture2D> Textures2DCollection;

	   Textures2DCollection textures2D;





	/*
	ID3D11DeviceContext** get_device_context()
	{
		return &d3dDeviceContext;
	}
	*/

	ID3D11DeviceContext* get_device_context();

	ID3DBlob* get_byte_code();

	ID3D11VertexShader* GetVS();

	ID3D11HullShader* GetHS();

	ID3D11DomainShader* GetDS();

	ID3D11GeometryShader* GetGS();

	ID3D11PixelShader* GetPS();

	ID3D11ComputeShader* GetCS();
	
//protected:
private:

	std::string name;

	std::vector<std::string> constant_buffers_names;

	std::vector<std::string> textures2D_names;


	ShaderTypes ShaderType;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dDeviceContext;


	ID3DBlob* pShaderCompilationBlob; // будет содержать шейдерный байт-код после компиляции шейдера


	// каким шейдером может стать:
	ID3D11VertexShader* vs;
	ID3D11HullShader* hs;
	ID3D11DomainShader* ds;
	ID3D11GeometryShader* gs;
	ID3D11PixelShader* ps;	
	ID3D11ComputeShader* cs;	


	ISetter* setter; // поле для хранения полиморфного объекта, тип которого будет определён во время выполнения метода create(...)	

};


class Specification
{
public:

	Specification(std::string FX_name, std::string shader_name, uint StartSlot, uint NumViews = 1);
	

public:

	std::string FX_name;
	std::string shader_name;
	
	uint StartSlot;
	uint NumViews;

};



class Texture2D
{

public:

	Texture2D() = default;

	Texture2D(char* file_name, const uint& StartSlot, const uint& NumViews, ID3D11Device* d3dDevice, ISetter* setter);

	Texture2D(ID3D11ShaderResourceView* SRV, const uint& StartSlot, const uint& NumViews, ID3D11Device* d3dDevice, ISetter* setter);

	Texture2D(char* file_name, const std::vector<Specification>& specifications);
	Texture2D(ID3D11ShaderResourceView* SRV, const std::vector<Specification>& specifications);

	Texture2D(char* file_name, Specification const & specification);
	Texture2D(ID3D11ShaderResourceView* SRV, Specification const & specification);

	void set();

	void apply();



private:

ID3D11ShaderResourceView* SRV;

effects::shader_system::Effects* FX_Group;

std::vector<Specification> specifications;

delegates_system::DelegatesSystem delegates;


char* file_name;

uint StartSlot;
uint NumViews;


ID3D11Device* d3dDevice;

ISetter* setter;



};


/*
class Textures2DSystem
{
public:

	void add(char* file_name, Specification const & specification);
	void add(ID3D11ShaderResourceView* SRV, Specification const & specification);



private:

	map_vector<std::string, Texture2D> textures;

};
*/

class IConstantBuffer
{

public:

	virtual std::string get_name() const = 0;
	virtual void set() = 0;
	virtual void* operator=(void* cb) = 0;
	virtual ID3D11Buffer* GetD3DBuffer() const = 0;
};


template<class T>
class ConstantBuffer: public IConstantBuffer
{

public:

	ConstantBuffer(const std::string& name, 
				   const uint& StartSlot, 
				   const T* content,
				   const uint& NumBuffers, 
				   ID3D11Device* d3dDevice,
				   ID3D11DeviceContext* d3dDeviceContext, 
				   ISetter* setter,  
				   D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC, 
				   UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE, 
				   UINT MiscFlags = 0);



	virtual std::string get_name() const; // перегрузка метода IConstantBuffer
	



	virtual void* operator=(void* cb); // перегрузка метода IConstantBuffer
	


	void create(D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC, UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE, UINT MiscFlags = 0);	


	void update();


	void set();  // перегрузка метода IConstantBuffer
	


	T& get_content();

	void set_content(const T& rhs);


	    __declspec(property(get = get_content, put = set_content)) T Content;



	ID3D11Buffer* GetD3DBuffer() const; // перегрузка метода IConstantBuffer
	
	
private:

	std::string name;

	typedef typename T type;

	T content;

	bool isSet;

	ISetter* setter;

	uint StartSlot; // индекс регистра cbuffer в HLSL (b#, cb#)
	uint NumBuffers;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dDeviceContext;

	ID3D11Buffer* d3dConstantBuffer;

};



template<class T> T& access(IConstantBuffer* buf);



/*
*
*	IMPLEMENTATION
*
*/



///
/// ------------- FX -----------------///
///

Effects::Effects()
{
	effects.SetKeys(&names);
}

ShaderSystem& Effects::operator[](const std::string& key)
{
	return effects[key];
}

ShaderSystem& Effects::operator[](const uint& idx)
{
	return effects[idx];
}

void Effects::Init(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext) // использовать этот метод, когда объект будет применять эффекты, поставляемые ShaderSystem
{
	this->d3dDevice = d3dDevice;
	this->d3dDeviceContext = d3dDeviceContext;
}


void Effects::Add(const std::string& name)
{
	names.push_back(name);
	effects.push_back(ShaderSystem(d3dDevice, d3dDeviceContext));
}

void Effects::Add(const std::string& name, ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext)
{
	names.push_back(name);
	effects.push_back(ShaderSystem(d3dDevice, d3dDeviceContext));
}

void Effects::Add(const std::string& name, const ShaderSystem& effect)
{
	names.push_back(name);
	effects.push_back(effect);
}

void Effects::Add(const std::string& name, const ShaderSystem& effect, ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext)
{
	ShaderSystem fx_buf(d3dDevice, d3dDeviceContext);
	fx_buf.shaders = effect.shaders;

	effects.push_back(fx_buf);
}

uint Effects::size()
{
	return effects.size();
}





///
/// ------------- ShaderSystem -----------------///
///

ShaderSystem::ShaderSystem() : VS_byte_code(0)
{

	shaders.SetKeys(&shader_names); // map_vector<>::SetKeys() - передаём указатель на вектор имён шейдеров, вместо копирования;
	// далее контейнер map_vector будет использовать имена шейдеров для доступа к шейдерам по ключу
}

ShaderSystem::ShaderSystem(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext)
{
	Init(d3dDevice, d3dDeviceContext);
}


void ShaderSystem::Init(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext) // использовать этот метод, когда объект будет применять эффекты, поставляемые ShaderSystem
{
	this->d3dDevice = d3dDevice;
	this->d3dDeviceContext = d3dDeviceContext;
}

void ShaderSystem::AddShader(std::string shader_name, const ShaderTypes& ShaderType, char* szFileName,
	LPCSTR szEntryPoint, LPCSTR szShaderModel)
{

	shader_names.push_back(shader_name);



	WCHAR* pwchar_str = 0;

	int chars = MultiByteToWideChar(CP_ACP, 0, szFileName, -1, NULL, 0);

	pwchar_str = new WCHAR[chars];

	MultiByteToWideChar(CP_ACP, 0, szFileName, -1, pwchar_str, chars);




	shaders.push_back(Shader(shader_name, d3dDevice, d3dDeviceContext, ShaderType, pwchar_str, szEntryPoint, szShaderModel));

	//shaders.push_back(Shader(shader_name, d3dDevice, d3dDeviceContext, ShaderType, pchar_to_pwchar(szFileName), szEntryPoint, szShaderModel));

	//shaders.insert(make_pair(shader_name, Shader(shader_name)));


	VS_byte_code = (!VS_byte_code) && (VertexShader == ShaderType) ? shaders[shaders.size() - 1].get_byte_code() : VS_byte_code; // вернуть шейдерный 
	//байт-код, если шейдер 
	// является вершинным

}



void ShaderSystem::set()
{
	for (uint i = 0; i < shaders.size(); i++)
		shaders[i].set();
}


ID3DBlob* ShaderSystem::get_VS_byte_code()
{
	return VS_byte_code;
}


void ShaderSystem::begin()
{
	begin_functions.launch();
}

void ShaderSystem::end()
{
	end_functions.launch();
}


void ShaderSystem::AddBeginFunction(delegates_system::Delegate f, std::string const & name)
{
	begin_functions.add(f, name);
}

void ShaderSystem::AddEndFunction(delegates_system::Delegate f, std::string const & name)
{
	end_functions.add(f, name);
}


void ShaderSystem::set_device(ID3D11Device* d3dDevice)
{
	this->d3dDevice = d3dDevice;
}

void ShaderSystem::set_device_context(ID3D11DeviceContext* d3dDeviceContext)
{
	this->d3dDeviceContext = d3dDeviceContext;
}


///
/// ------------- Shader -----------------///
///


Shader::Shader(std::string shader_name, ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext,
	const ShaderTypes& ShaderType, WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel)
	: name(shader_name), pShaderCompilationBlob(0)
{
	constant_buffers.SetKeys(&constant_buffers_names); // связываем адрес поля Shader::constant_buffers_names с map_vector::keys
	textures2D.SetKeys(&textures2D_names);


	this->d3dDevice = d3dDevice;
	this->d3dDeviceContext = d3dDeviceContext;
	this->ShaderType = ShaderType;

	create(szFileName, szEntryPoint, szShaderModel);

};


Shader::Shader(const Shader& rhs)
{

	if (this != &rhs)
	{
		*this = rhs;
		constant_buffers.SetKeys(&constant_buffers_names);
		textures2D.SetKeys(&textures2D_names);
	}

	//cout<<"Shader::copy constructor()"<<"\сn\n";

}



template<class T>
void Shader::AddConstantBuffer(const std::string& buffer_name,
	const uint& StartSlot,
	const T* content,
	const uint& NumBuffers,
	D3D11_USAGE Usage,
	UINT CPUAccessFlags,
	UINT MiscFlags)
{
	constant_buffers.push_back(new ConstantBuffer<T>(buffer_name, StartSlot, content, NumBuffers, d3dDevice, d3dDeviceContext,
		setter, Usage, CPUAccessFlags, MiscFlags)); // создаётся полиморфный объект
	// IConstantBuffer* == ConstantBuffer<T> и добавляется в коллекцию КБ
	// map_vector<std::string, IConstantBuffer*> constant_buffers

	constant_buffers_names.push_back(buffer_name);
	//ConstantBuffers.insert(make_pair("name", ConstantBuffer<T>));
}


void Shader::AddTexture2D(char* file_name, const uint& StartSlot, const uint& NumViews)
{

	textures2D.push_back(Texture2D(file_name, StartSlot, NumViews, d3dDevice, setter));

}


HRESULT Shader::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel)
{
	HRESULT hr = S_OK;

	if (pShaderCompilationBlob)
	{
		pShaderCompilationBlob->Release();
		pShaderCompilationBlob = 0;
	}

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob = 0;
	hr = D3DCompileFromFile(szFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, &pShaderCompilationBlob, &pErrorBlob);

	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();


	return S_OK;
}


void Shader::create(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel)
{
	if (SUCCEEDED(CompileShaderFromFile(szFileName, szEntryPoint, szShaderModel)))


		// выбор установщика на основе типа шейдера;
		// поскольку тип шейдера определяет и тип констнантного буфера (devCon->XXSetConstantBuffers()),
		// то имеет смысл единожды произвести данное сопоставление и сохранить его;
		// именно поэтому интерфейс ISetter задаёт метод для установки шейдера и метод для установки КБ;
		// далее при создании КБ в качестве одного из параметров будет передан установщик уже выбранного
		// типа XXXShaderSetter, содержащий соответствующий оператор утсановки КБ (devCon->XXSetConstantBuffers()),
		// который будет выполнен во время вызова метода IConstantBuffer<T>::set()
		switch (this->ShaderType)
	{

		case VertexShader: d3dDevice->CreateVertexShader(pShaderCompilationBlob->GetBufferPointer(),
			pShaderCompilationBlob->GetBufferSize(), NULL, &vs);

			setter = new VertexShaderSetter(get_device_context(), vs);

			break;

		case HullShader: d3dDevice->CreateHullShader(pShaderCompilationBlob->GetBufferPointer(),
			pShaderCompilationBlob->GetBufferSize(), NULL, &hs);

			setter = new HullShaderSetter(get_device_context(), hs);

			break;

		case DomainShader: d3dDevice->CreateDomainShader(pShaderCompilationBlob->GetBufferPointer(),
			pShaderCompilationBlob->GetBufferSize(), NULL, &ds);

			setter = new DomainShaderSetter(get_device_context(), ds);

			break;

		case GeometryShader: d3dDevice->CreateGeometryShader(pShaderCompilationBlob->GetBufferPointer(),
			pShaderCompilationBlob->GetBufferSize(), NULL, &gs);

			setter = new GeometryShaderSetter(get_device_context(), gs);

			break;

		case PixelShader: d3dDevice->CreatePixelShader(pShaderCompilationBlob->GetBufferPointer(),
			pShaderCompilationBlob->GetBufferSize(), NULL, &ps);

			setter = new PixelShaderSetter(get_device_context(), ps);

			break;

		case ComputeShader: d3dDevice->CreateComputeShader(pShaderCompilationBlob->GetBufferPointer(),
			pShaderCompilationBlob->GetBufferSize(), NULL, &cs);

			setter = new ComputeShaderSetter(get_device_context(), cs);

			break;

	};

}

void Shader::set()
{
	setter->set_shader(); // сперва устанавливается шейдер, т.к. это задаёт соответсвие для КБ, т.е. с каким шейдером будет связан КБ

	for (uint i = 0; i < constant_buffers.size(); i++)
		constant_buffers[i]->set();

	for (uint i = 0; i < textures2D.size(); i++)
		textures2D[i].set();

}


void Shader::set_Texture2D(ID3D11ShaderResourceView* const * SRV, const uint& StartSlot, const uint& NumViews)
{
	setter->set_texture(StartSlot, NumViews, SRV);
}



std::string& Shader::get_name()
{
	return name;
}


ID3D11DeviceContext* Shader::get_device_context()
{
	return d3dDeviceContext;
}

ID3DBlob* Shader::get_byte_code()
{
	return pShaderCompilationBlob;
}

ID3D11VertexShader* Shader::GetVS()
{
	return vs;
}

ID3D11HullShader* Shader::GetHS()
{
	return hs;
}

ID3D11DomainShader* Shader::GetDS()
{
	return ds;
}

ID3D11GeometryShader* Shader::GetGS()
{
	return gs;
}

ID3D11PixelShader* Shader::GetPS()
{
	return ps;
}

ID3D11ComputeShader* Shader::GetCS()
{
	return cs;
}






///
/// ------------- Texture2D ---------------///
///


Specification::Specification(std::string FX_name, std::string shader_name, uint StartSlot, uint NumViews)
{
	this->FX_name = FX_name;
	this->shader_name = shader_name;

	this->StartSlot = StartSlot;
	this->NumViews = NumViews;

}


Texture2D::Texture2D(char* file_name, const uint& StartSlot, const uint& NumViews, ID3D11Device* d3dDevice, ISetter* setter)
{
	this->StartSlot = StartSlot;
	this->NumViews = NumViews;
	this->d3dDevice = d3dDevice;

	this->setter = setter;

	// Преобразование из char* в WCHAR*

	const WCHAR* fn = 0;

	//fn = string_facilities::pchar_to_pwchar(file_name);


	WCHAR* pwchar_str = 0;

	int chars = MultiByteToWideChar(CP_ACP, 0, file_name, -1, NULL, 0);

	pwchar_str = new WCHAR[chars];

	MultiByteToWideChar(CP_ACP, 0, file_name, -1, pwchar_str, chars);


	fn = pwchar_str;


	HR(CreateDDSTextureFromFile(d3dDevice, fn, NULL, &SRV));

	//D3DX11CreateShaderResourceViewFromFile(d3dDevice, fn , 0, 0, &pDiffuseMap, 0); // D3DX  utility library is deprecated for Windows 8


}


Texture2D::Texture2D(ID3D11ShaderResourceView* SRV, const uint& StartSlot, const uint& NumViews, ID3D11Device* d3dDevice, ISetter* setter)
{
	this->StartSlot = StartSlot;
	this->NumViews = NumViews;
	this->d3dDevice = d3dDevice;

	this->setter = setter;

	this->SRV = SRV;
}


Texture2D::Texture2D(char* file_name, const std::vector<Specification>& specifications)
{

	// Преобразование из char* в WCHAR*

	const WCHAR* fn = 0;

	//fn = string_facilities::pchar_to_pwchar(file_name);


	WCHAR* pwchar_str = 0;

	int chars = MultiByteToWideChar(CP_ACP, 0, file_name, -1, NULL, 0);

	pwchar_str = new WCHAR[chars];

	MultiByteToWideChar(CP_ACP, 0, file_name, -1, pwchar_str, chars);


	fn = pwchar_str;


	HR(CreateDDSTextureFromFile(d3dDevice, fn, NULL, &SRV));

	for (const auto& s : specifications)
	{
		Delegate d = Delegate::Create<void>(&((*FX_Group)[s.FX_name].shaders[s.shader_name]), &Shader::set_Texture2D);

		d.bind_args(&SRV, &s.StartSlot, &s.NumViews);

		delegates.add(d, "Shader::set_Texture2D");

	}


}


Texture2D::Texture2D(ID3D11ShaderResourceView* SRV, const std::vector<Specification>& specifications)
{
	this->SRV = SRV;

	for (const auto& s : specifications)
	{
		Delegate d = Delegate::Create<void>(&((*FX_Group)[s.FX_name].shaders[s.shader_name]), &Shader::set_Texture2D);

		d.bind_args(&SRV, &s.StartSlot, &s.NumViews);

		delegates.add(d, "Shader::set_Texture2D");

	}

}



Texture2D::Texture2D(char* file_name, Specification const & specification)
{

	// Преобразование из char* в WCHAR*

	const WCHAR* fn = 0;

	//fn = string_facilities::pchar_to_pwchar(file_name);


	WCHAR* pwchar_str = 0;

	int chars = MultiByteToWideChar(CP_ACP, 0, file_name, -1, NULL, 0);

	pwchar_str = new WCHAR[chars];

	MultiByteToWideChar(CP_ACP, 0, file_name, -1, pwchar_str, chars);


	fn = pwchar_str;


	HR(CreateDDSTextureFromFile(d3dDevice, fn, NULL, &SRV));



	const auto& s = specification;

	Delegate d = Delegate::Create<void>(&((*FX_Group)[s.FX_name].shaders[s.shader_name]), &Shader::set_Texture2D);

	d.bind_args(&SRV, &s.StartSlot, &s.NumViews);

	delegates.add(d, "Shader::set_Texture2D");

}


Texture2D::Texture2D(ID3D11ShaderResourceView* SRV, Specification const & specification)
{
	this->SRV = SRV;


	const auto& s = specification;

	Delegate d = Delegate::Create<void>(&((*FX_Group)[s.FX_name].shaders[s.shader_name]), &Shader::set_Texture2D);

	d.bind_args(&SRV, &s.StartSlot, &s.NumViews);

	delegates.add(d, "Shader::set_Texture2D");

}



void Texture2D::set()
{
	setter->set_texture(StartSlot, NumViews, &SRV); // привязать текстуру к шейдеру, задаваемому установщиком setter
}


void Texture2D::apply()
{
	delegates.launch();
}


///
/// ------------- ConstantBuffer -----------------///
///



template<class T>
ConstantBuffer<T>::ConstantBuffer(const std::string& name,
	const uint& StartSlot,
	const T* content,
	const uint& NumBuffers,
	ID3D11Device* d3dDevice,
	ID3D11DeviceContext* d3dDeviceContext,
	ISetter* setter,
	D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC,
	UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
	UINT MiscFlags = 0)
{
	this->name = name;
	this->setter = setter;
	this->StartSlot = StartSlot;

	if (content)
	{
		this->content = *content; // на случай необходимости инициализировать КБ
		isSet = true;
	}
	else
	{
		isSet = false;
	}

	this->NumBuffers = NumBuffers;
	this->d3dDevice = d3dDevice;
	this->d3dDeviceContext = d3dDeviceContext;

	if (this->d3dDevice)

		create(Usage, CPUAccessFlags, MiscFlags);
}

template<class T>
std::string ConstantBuffer<T>::get_name() const // перегрузка метода IConstantBuffer
{
	return name;
}



template<class T>
void* ConstantBuffer<T>::operator=(void* cb) // перегрузка метода IConstantBuffer
{
	content = *static_cast<T*>(cb);
	return static_cast<void*>(&content);
}

template<class T>
void ConstantBuffer<T>::create(D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC, UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE, UINT MiscFlags = 0)
{
	D3D11_BUFFER_DESC bdesc;

	bdesc.Usage = Usage;
	bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bdesc.CPUAccessFlags = CPUAccessFlags;
	bdesc.MiscFlags = MiscFlags;
	bdesc.ByteWidth = sizeof(T);

	if (isSet)
	{
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = &content;

		d3dDevice->CreateBuffer(&bdesc, &InitData, &d3dConstantBuffer);
	}

	else

		d3dDevice->CreateBuffer(&bdesc, NULL, &d3dConstantBuffer);
}


template<class T>
void ConstantBuffer<T>::update()
{
	D3D11_MAPPED_SUBRESOURCE bufferMapping;

	d3dDeviceContext->Map(d3dConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferMapping);

	T* joinning_ptr = (T*)bufferMapping.pData;

	*joinning_ptr = content; // сопоставить данные content с буфером d3dConstantBuffer; дальнейшее сопоставление с шейдером будет задано, тем какой шейдер будет установлен перед установкой данного КБ

	d3dDeviceContext->Unmap(d3dConstantBuffer, 0);
}


template<class T>
void ConstantBuffer<T>::set()  // перегрузка метода IConstantBuffer
{
	update();

	setter->set_buffer(d3dConstantBuffer, StartSlot, NumBuffers); // StartSlot - индекс регистра cbuffer в HLSL (b#, cb#)
}


template<class T>
T& ConstantBuffer<T>::get_content()
{
	return content;
}

template<class T>
void ConstantBuffer<T>::set_content(const T& rhs)
{
	content = rhs;
}


template<class T>
ID3D11Buffer* ConstantBuffer<T>::GetD3DBuffer() const // перегрузка метода IConstantBuffer
{
	return d3dConstantBuffer;
}


template<class T> T& access(IConstantBuffer* buf)
{
	return static_cast<ConstantBuffer<T>*>(buf)->Content;
}


///
/// ------------- Setters -----------------///
///


/// VS


VertexShaderSetter::VertexShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11VertexShader* VS)
{
	this->d3dDeviceContext = d3dDeviceContext; this->VS = VS;
}

void VertexShaderSetter::set_shader()
{
	d3dDeviceContext->VSSetShader(VS, NULL, 0);
}

void VertexShaderSetter::set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers)
{
	d3dDeviceContext->VSSetConstantBuffers(StartSlot, NumBuffers, &d3dBuf);
}


void VertexShaderSetter::set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV)
{
	d3dDeviceContext->VSSetShaderResources(StartSlot, NumViews, SRV);
}

/// HS

HullShaderSetter::HullShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11HullShader* HS)
{
	this->d3dDeviceContext = d3dDeviceContext;
	this->HS = HS;
}

void HullShaderSetter::set_shader()
{
	d3dDeviceContext->HSSetShader(HS, NULL, 0);
}

void HullShaderSetter::set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers)
{
	d3dDeviceContext->HSSetConstantBuffers(StartSlot, NumBuffers, &d3dBuf);
}


void HullShaderSetter::set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV)
{
	d3dDeviceContext->HSSetShaderResources(StartSlot, NumViews, SRV);
}


/// DS

DomainShaderSetter::DomainShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11DomainShader* DS)
{
	this->d3dDeviceContext = d3dDeviceContext; this->DS = DS;
}

void DomainShaderSetter::set_shader()
{
	d3dDeviceContext->DSSetShader(DS, NULL, 0);
}

void DomainShaderSetter::set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers)
{
	d3dDeviceContext->DSSetConstantBuffers(StartSlot, NumBuffers, &d3dBuf);
}


void DomainShaderSetter::set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV)
{
	d3dDeviceContext->DSSetShaderResources(StartSlot, NumViews, SRV);
}



/// GS


GeometryShaderSetter::GeometryShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11GeometryShader* GS)
{
	this->d3dDeviceContext = d3dDeviceContext; this->GS = GS;
}

void GeometryShaderSetter::set_shader()
{
	d3dDeviceContext->GSSetShader(GS, NULL, 0);
}

void GeometryShaderSetter::set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers)
{
	d3dDeviceContext->GSSetConstantBuffers(StartSlot, NumBuffers, &d3dBuf);
}


void GeometryShaderSetter::set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV)
{
	d3dDeviceContext->GSSetShaderResources(StartSlot, NumViews, SRV);
}



/// PS

PixelShaderSetter::PixelShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11PixelShader* PS)
{
	this->d3dDeviceContext = d3dDeviceContext; this->PS = PS;
}

void PixelShaderSetter::set_shader()
{
	d3dDeviceContext->PSSetShader(PS, NULL, 0);
}

void PixelShaderSetter::set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers)
{
	d3dDeviceContext->PSSetConstantBuffers(StartSlot, NumBuffers, &d3dBuf);
}


void PixelShaderSetter::set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV)
{
	d3dDeviceContext->PSSetShaderResources(StartSlot, NumViews, SRV);
}

/// CS

ComputeShaderSetter::ComputeShaderSetter(ID3D11DeviceContext* d3dDeviceContext, ID3D11ComputeShader* CS)
{
	this->d3dDeviceContext = d3dDeviceContext; this->CS = CS;
}

void ComputeShaderSetter::set_shader()
{
	d3dDeviceContext->CSSetShader(CS, NULL, 0);
}

void ComputeShaderSetter::set_buffer(ID3D11Buffer* d3dBuf, const uint& StartSlot, const uint& NumBuffers)
{
	d3dDeviceContext->CSSetConstantBuffers(StartSlot, NumBuffers, &d3dBuf);
}


void ComputeShaderSetter::set_texture(const uint & StartSlot, const uint& NumViews, ID3D11ShaderResourceView* const * SRV)
{
	d3dDeviceContext->CSSetShaderResources(StartSlot, NumViews, SRV);
}



} // namespace shader_system

} // namespace effects


#endif




