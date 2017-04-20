// Протосфера 0.007.cpp : Defines the entry point for the application.
//

#include "Протосфера.h"

#include "Physics.h"

//#include "ShaderSystem.h"
//#include "DelegatesSystem.h"

#include <sstream>
#include <stdio.h>

#include "ShadowMap.h"

//#include "map_vector.h"




//#include <d3dcompiler.h>

//#define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)


using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

using namespace effects;
using namespace effects::shader_system;

using namespace delegates_system;



///
/// ------------- map_vector -----------------///
///

/*

// Метод для определения индекса заданного ключа в векторе ключей;
// нужно для реализации ассоциативного доступа к вектору (подобно доступу к отображению)
template<class Key, class Val>
uint map_vector<Key, Val>::index_of(const std::vector<Key>& key_vector, const Key& search_val)
{

	std::vector<Key>::const_iterator it =
		search_n(key_vector.begin(), key_vector.end(), 1, search_val);

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
void map_vector<Key, Val>::SetKeys(std::vector<Key>* keys)
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

*/



namespace effects
{

namespace shader_system
{

	/*

///
/// ------------- map_vector -----------------///
///


// Метод для определения индекса заданного ключа в векторе ключей;
// нужно для реализации ассоциативного доступа к вектору (подобно доступу к отображению)
template<class Key, class Val>
uint map_vector<Key, Val>::index_of(const std::vector<Key>& key_vector, const Key& search_val)
{

  std::vector<Key>::const_iterator it=  
				search_n(key_vector.begin(), key_vector.end(), 1, search_val);
  
		return it-key_vector.cbegin();
				
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
void map_vector<Key, Val>::SetKeys(std::vector<Key>* keys)
{
	this->keys= keys;
}

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
	this->d3dDevice= d3dDevice;
	this->d3dDeviceContext= d3dDeviceContext;
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
	fx_buf.shaders= effect.shaders;

	effects.push_back(fx_buf);
}

uint Effects::size()
{
	return effects.size();
}





///
/// ------------- ShaderSystem -----------------///
///

ShaderSystem::ShaderSystem(): VS_byte_code(0)
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
	this->d3dDevice= d3dDevice;
	this->d3dDeviceContext= d3dDeviceContext;
}

void ShaderSystem::AddShader(std::string shader_name, const ShaderTypes& ShaderType, char* szFileName, 
							                       LPCSTR szEntryPoint, LPCSTR szShaderModel)
{

	shader_names.push_back(shader_name);



	WCHAR* pwchar_str= 0;

				int chars= MultiByteToWideChar(CP_ACP, 0, szFileName, -1, NULL, 0);

				pwchar_str= new WCHAR[chars];

				MultiByteToWideChar(CP_ACP, 0, szFileName, -1, pwchar_str, chars);




shaders.push_back(Shader(shader_name, d3dDevice, d3dDeviceContext, ShaderType, pwchar_str, szEntryPoint, szShaderModel));
	
//shaders.push_back(Shader(shader_name, d3dDevice, d3dDeviceContext, ShaderType, pchar_to_pwchar(szFileName), szEntryPoint, szShaderModel));

	//shaders.insert(make_pair(shader_name, Shader(shader_name)));


	VS_byte_code= (!VS_byte_code) && (VertexShader == ShaderType) ? shaders[shaders.size()-1].get_byte_code() : VS_byte_code; // вернуть шейдерный 
																													//байт-код, если шейдер 
																											// является вершинным
		
}



void ShaderSystem::set()
{
	for(uint i= 0; i < shaders.size(); i++)
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
	this->d3dDevice= d3dDevice;
}

void ShaderSystem::set_device_context(ID3D11DeviceContext* d3dDeviceContext)
{
	this->d3dDeviceContext= d3dDeviceContext;
}


///
/// ------------- Shader -----------------///
///


Shader::Shader(std::string shader_name, ID3D11Device* d3dDevice,	ID3D11DeviceContext* d3dDeviceContext, 
		const ShaderTypes& ShaderType, WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel)
: name(shader_name), pShaderCompilationBlob(0)
{
	constant_buffers.SetKeys(&constant_buffers_names); // связываем адрес поля Shader::constant_buffers_names с map_vector::keys
	textures2D.SetKeys(&textures2D_names);

		
	this->d3dDevice= d3dDevice;
	this->d3dDeviceContext= d3dDeviceContext;
	this->ShaderType= ShaderType;

			create(szFileName, szEntryPoint, szShaderModel);

};


Shader::Shader(const Shader& rhs)
{
	
	if(this != &rhs)
	{
		*this= rhs;			
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

	if(pShaderCompilationBlob) 
	{pShaderCompilationBlob->Release(); 
	pShaderCompilationBlob= 0;}

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob = 0;
    hr = D3DCompileFromFile( szFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, &pShaderCompilationBlob, &pErrorBlob );
   
	if( FAILED(hr) )
    {
        if( pErrorBlob != NULL)
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();


    return S_OK;
}


void Shader::create(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel)
{
	if(SUCCEEDED(CompileShaderFromFile(szFileName, szEntryPoint, szShaderModel)))


	// выбор установщика на основе типа шейдера;
	// поскольку тип шейдера определяет и тип констнантного буфера (devCon->XXSetConstantBuffers()),
	// то имеет смысл единожды произвести данное сопоставление и сохранить его;
	// именно поэтому интерфейс ISetter задаёт метод для установки шейдера и метод для установки КБ;
	// далее при создании КБ в качестве одного из параметров будет передан установщик уже выбранного
    // типа XXXShaderSetter, содержащий соответствующий оператор утсановки КБ (devCon->XXSetConstantBuffers()),
	// который будет выполнен во время вызова метода IConstantBuffer<T>::set()
	switch(this->ShaderType)
	{
				
		case VertexShader: d3dDevice->CreateVertexShader(pShaderCompilationBlob->GetBufferPointer(),
							   pShaderCompilationBlob->GetBufferSize(), NULL, &vs);

							setter= new VertexShaderSetter(get_device_context(), vs);

			break;

		case HullShader: d3dDevice->CreateHullShader(pShaderCompilationBlob->GetBufferPointer(),
							 pShaderCompilationBlob->GetBufferSize(), NULL, &hs);

							setter= new HullShaderSetter(get_device_context(), hs);

			break;

		case DomainShader: d3dDevice->CreateDomainShader(pShaderCompilationBlob->GetBufferPointer(), 
							   pShaderCompilationBlob->GetBufferSize(), NULL, &ds);

							setter= new DomainShaderSetter(get_device_context(), ds);

			break;

		case GeometryShader: d3dDevice->CreateGeometryShader(pShaderCompilationBlob->GetBufferPointer(), 
								 pShaderCompilationBlob->GetBufferSize(), NULL, &gs);

							setter= new GeometryShaderSetter(get_device_context(), gs);

			break;

		case PixelShader: d3dDevice->CreatePixelShader(pShaderCompilationBlob->GetBufferPointer(), 
							  pShaderCompilationBlob->GetBufferSize(), NULL, &ps);

							setter= new PixelShaderSetter(get_device_context(), ps);

			break;

		case ComputeShader: d3dDevice->CreateComputeShader(pShaderCompilationBlob->GetBufferPointer(),
								pShaderCompilationBlob->GetBufferSize(), NULL, &cs);

							setter= new ComputeShaderSetter(get_device_context(), cs);

			break;
			
	};

}

void Shader::set()
{
	setter->set_shader(); // сперва устанавливается шейдер, т.к. это задаёт соответсвие для КБ, т.е. с каким шейдером будет связан КБ

	for(uint i= 0; i < constant_buffers.size(); i++)
		 constant_buffers[i]->set();

	for(uint i= 0; i < textures2D.size(); i++)
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
	this->StartSlot= StartSlot;
	this->NumViews= NumViews;
	this->d3dDevice= d3dDevice;

	this->setter= setter;

		// Преобразование из char* в WCHAR*

			const WCHAR* fn= 0;

			//fn = string_facilities::pchar_to_pwchar(file_name);


		WCHAR* pwchar_str= 0;

				int chars= MultiByteToWideChar(CP_ACP, 0, file_name, -1, NULL, 0);

				pwchar_str= new WCHAR[chars];

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
	this->name= name;
	this->setter= setter;
	this->StartSlot= StartSlot;
	
	if(content)
	{
		this->content= *content; // на случай необходимости инициализировать КБ
		isSet= true;
	}
	else
	{
		isSet= false;
	}
	
	this->NumBuffers= NumBuffers;
	this->d3dDevice= d3dDevice;
	this->d3dDeviceContext= d3dDeviceContext;

	if(this->d3dDevice) 
		
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

	bdesc.Usage= Usage;
	bdesc.BindFlags= D3D11_BIND_CONSTANT_BUFFER;
	bdesc.CPUAccessFlags= CPUAccessFlags;
	bdesc.MiscFlags= MiscFlags;
	bdesc.ByteWidth= sizeof(T);
	
	if(isSet)
	{
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem= &content;
		  
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

		T* joinning_ptr = (T*) bufferMapping.pData;

		*joinning_ptr= content; // сопоставить данные content с буфером d3dConstantBuffer; дальнейшее сопоставление с шейдером будет задано, тем какой шейдер будет установлен перед установкой данного КБ

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
	content= rhs;
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
	this->d3dDeviceContext= d3dDeviceContext; this->VS= VS;
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
	this->d3dDeviceContext= d3dDeviceContext; 
	this->HS= HS;
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
	this->d3dDeviceContext= d3dDeviceContext; this->DS= DS;
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
	this->d3dDeviceContext= d3dDeviceContext; this->GS= GS;
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
	this->d3dDeviceContext= d3dDeviceContext; this->PS= PS;
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
	this->d3dDeviceContext= d3dDeviceContext; this->CS= CS;
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











class Application: public Protosphere
{

public:

	Application();

			void InitScene();

			void OnResize();


			virtual void UpdateScene(float dt);

			virtual void RenderScene();


		LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);


	XMMATRIX World, View, Proj, WVP, TexMtx;
	

	POINT aim;
	POINT mOldMousePos;

	//ID3DX10Font* pFont;
	string stats;


	//void TextOutput(ID3DX10Font* const font, const string& text, const DWORD& left = 4, const DWORD& top = 4);



	float theta;
	float phi;

	XMVECTOR EyePos;
	float radius;

		
};

XMVECTOR 
RED= XMVectorSet(1, 0, 0, 0),
GREEN= XMVectorSet(0, 1, 0, 0),
BLUE= XMVectorSet(0, 0, 1, 0);



ID3DX11EffectVectorVariable* fxLightDir;








float forceY= -1e-2;

float h = 0;

D3D11_FILL_MODE fill_mode = D3D11_FILL_SOLID;

bool isBounce = true;

bool isApplyGravity = false;


XMMATRIX mLookAt;


namespace scene
{

// Для наложения теней
namespace shadow_mapping
{

	ShadowMap * depthMap;

	XMMATRIX LightVolumeProj;
	XMMATRIX LightWVP; 

	

}

namespace fx
{
	
	ShaderSystem RenderFX;
	ShaderSystem DepthMapBuildFX;
	
	Effect11 TexFX;
	Effect11 ShadowFX;
	
}

namespace light_sources
{

	LightSource Parallel;

}

namespace objects
{

  Mesh model1;

	 Camera camera;

 Cube box;
 
   Sphere sphere;

       Landscape terrain;

   Cloth cloth;

   InfoVectors infoV;

     Grid plane;


	 Sphere sphere2;

	 Grid plane2;

}


	namespace reflector_types
	{

		namespace VS_render
		{
			struct VS_cbPerScene
			{
				XMMATRIX WV;
				XMMATRIX View;

				XMMATRIX LightWVP; // for shadow mapping
			};


			struct VS_cbPerObject   
			{
				XMMATRIX World;
				XMMATRIX LWVP;

				XMMATRIX TexMtx;
			};


		}


		namespace PS_render
		{

			struct PS_cbPerFrame
			{	
				XMMATRIX WV;
	
				XMVECTOR LightDir;
				XMVECTOR LightPos;
				XMVECTOR pos_test2;

				XMVECTOR L_ambient;
				XMVECTOR L_diffuse;
				XMVECTOR L_specular;

				int LightSourceType; 
				LightSource light;
				LightSource light2;
				XMVECTOR EyePosW;
			};

		}


		namespace VS_build_depthmap
		{
			struct VS_build_depthmap_cbPerFrame    
			{
				XMMATRIX LightWVP;
			};
		}


	} // namespace reflector_types




} // namespace scene




using scene::reflector_types::VS_render::VS_cbPerScene;
using scene::reflector_types::VS_render::VS_cbPerObject;
using scene::reflector_types::PS_render::PS_cbPerFrame;
using scene::reflector_types::VS_build_depthmap::VS_build_depthmap_cbPerFrame;

using scene::fx::RenderFX;
using scene::fx::DepthMapBuildFX;





//Application::Application(): theta(Pi/4), phi(0.25*Pi), radius(8.0f)
Application::Application(): theta(0.0f*Pi), phi(0.0f*Pi), radius(20.0f)
{

		View= DirectX::XMMatrixIdentity();
		Proj= DirectX::XMMatrixIdentity();
		WVP= DirectX::XMMatrixIdentity();
		TexMtx= DirectX::XMMatrixIdentity();

}


void Application::InitScene()
{

	/*
	map_vector<string, int> mv;

	vector<string> keys;
	mv.SetKeys(&keys);

	keys.push_back("i1");

	mv.push_back(5);
	*/



	//------ Наложение теней

	scene::shadow_mapping::depthMap = new ShadowMap;
	scene::shadow_mapping::depthMap->Initialize(d3d->Device, d3d->DeviceContext);
	//scene::shadow_mapping::depthMap->Create_DSV_SRV_VP_For_depthMap(ClientWidth, ClientHeight);
	scene::shadow_mapping::depthMap->Create_DSV_SRV_VP_For_depthMap(1024, 1024);
	scene::shadow_mapping::depthMap->Create_RTV_SRV(1024, 1024);
		
		//XMMatrixOrthoLH(&scene::shadow_mapping::LightVolumeProj, 30.0f, 30.0f, 1.0f, 100.0f);
	float aspectratio = (float)ClientWidth/ClientHeight;
	scene::shadow_mapping::LightVolumeProj= XMMatrixPerspectiveFovLH(0.25*Pi, aspectratio, 0.5f, 1e3f);





//---------------------------------------ShaderSystem-------------------------------------//

	//--------- Шейдеры (эффекты) общие для всех объектов сцены

		//--------RenderFX

		RenderFX.Init(d3d->Device, d3d->DeviceContext);
	
		RenderFX.AddShader("VS", VertexShader, "shaders\\HLSL\\render_VS.hlsl", "VS", "vs_5_0");
			RenderFX.shaders["VS"].AddConstantBuffer<VS_cbPerScene>("vs_cbPerScene", 0); // b0
			RenderFX.shaders["VS"].AddConstantBuffer<VS_cbPerObject>("vs_cbPerObject", 1); // b1

		RenderFX.AddShader("PS", PixelShader, "shaders\\HLSL\\render_PS.hlsl", "PS", "ps_5_0");
			RenderFX.shaders["PS"].AddConstantBuffer<PS_cbPerFrame>("ps_cbPerFrame", 0); // b0



		//-------DepthMapBuildFX

		DepthMapBuildFX.Init(d3d->Device, d3d->DeviceContext);
		
		DepthMapBuildFX.AddShader("VS", VertexShader, "shaders\\HLSL\\build_depthmap_VS.hlsl", "VS", "vs_5_0");
			DepthMapBuildFX.shaders["VS"].AddConstantBuffer<VS_build_depthmap_cbPerFrame>("vs_build_depthmap_cbPerFrame", 0); // b0

		DepthMapBuildFX.AddShader("PS", PixelShader, "shaders\\HLSL\\build_depthmap_PS.hlsl", "PS", "ps_5_0");

		DepthMapBuildFX.AddBeginFunction(Delegate::Create<void>(scene::shadow_mapping::depthMap, &ShadowMap::Set_DSV_without_RTV_Set_VP), "Set_DSV_without_RTV_Set_VP");
		DepthMapBuildFX.AddEndFunction(Delegate::Create<void>(scene::shadow_mapping::depthMap, &ShadowMap::GenerateMipsForSRV), "GenerateMipsForSRV");
		DepthMapBuildFX.AddEndFunction(Delegate::Create<void>(d3d, &D3D::Reset_RTV_DSV_VP), "Reset_RTV_DSV_VP");

		DepthMapBuildFX.end_functions["Reset_RTV_DSV_VP"].bind_args(&ClientWidth, &ClientHeight);

			
//----------Сцена--------------------------------------


	//----------- плоскость

		scene::objects::plane2.Init(d3d->Device, d3d->DeviceContext, et_ShaderSystem);
		scene::objects::plane2.ConstructGeometry(20, 20, 1);		
		
		scene::objects::plane2.FX.Add("DepthMapBuildFX", DepthMapBuildFX);		
		scene::objects::plane2.FX.Add("RenderFX", RenderFX);

		scene::objects::plane2.FX["RenderFX"].shaders["PS"].AddTexture2D("resources\\textures\\cloth.DDS", 1); // t1
		
		scene::objects::plane2.VertexInputLayoutCreate(scene::objects::plane2.FX["RenderFX"].shaders["VS"].get_byte_code());
		
		//scene::objects::plane2.Position= XMVectorSet(20, 0, 0, 0);
		//scene::objects::plane2.RotateAroundXAxis(Pi/2);


	//----------- сфера

	scene::objects::sphere2.Init(d3d->Device, d3d->DeviceContext, et_ShaderSystem);	
	scene::objects::sphere2.ConstructGeometry(2, 4, 40, 40);		
	
	scene::objects::sphere2.FX.Add("DepthMapBuildFX", DepthMapBuildFX);	
	scene::objects::sphere2.FX.Add("RenderFX", RenderFX);

	scene::objects::sphere2.FX["RenderFX"].shaders["PS"].AddTexture2D("resources\\textures\\Protosphere.DDS", 1); // t1

	scene::objects::sphere2.VertexInputLayoutCreate(scene::objects::sphere2.FX["RenderFX"].shaders["VS"].get_byte_code());

	scene::objects::sphere2.Position= XMVectorSet(0, 3, 0, 0);



//-------------------------Effect11-------------------------------//


	


	//----- Эффект и техника для наложения теней

	scene::fx::ShadowFX.Create(d3d->Device, d3d->DeviceContext, "build_depthmap.fx", "DepthMapCreateTech");






	//----- Эффект и техника всех объектов сцены Effect11: освещение, текстурирование

	scene::fx::TexFX.Create(d3d->Device, d3d->DeviceContext, "shaders\\fx\\tex.fx", "TexTech");
	



	//----- Сфера

	h = 3;

	scene::objects::sphere.InitFromExistingData(d3d->Device, d3d->DeviceContext, scene::fx::TexFX.GetEffect(), scene::fx::TexFX.GetTechnique());

	scene::objects::sphere.ConstructGeometry(2, 4, 40, 40);
	//scene::objects::sphere.ConstructGeometry(1, 2, 10, 10);
	//scene::objects::sphere.Position= XMVectorSet(0, 5, 0);
	scene::objects::sphere.SetPosition(0, h, 0);
	//scene::objects::sphere.m= 1-1e-1*5;
	//scene::objects::sphere.TextureDiffuseApply("hearts.jpg");
	scene::objects::sphere.FX11.TextureDiffuseApply("resources\\textures\\Protosphere.dds");
	scene::objects::sphere.FX11.TextureSpecularApply("resources\\textures\\defaultspec.dds");


	//----- Плоскость

	scene::objects::plane.Init(d3d->Device, d3d->DeviceContext, "draw_depthmap.fx", "DrawDepthMapTech");
	scene::objects::plane.ConstructGeometry(20, 35, 1);
	//scene::objects::plane.TextureDiffuseApply("crate2.jpg");
	scene::objects::plane.Position= XMVectorSet(20, 0, 25, 0);
	scene::objects::plane.RotateAroundXAxis(Pi/2);



	//----- Источник света

	scene::light_sources::Parallel.InitFromExistingData(Spot, d3d->Device, d3d->DeviceContext, scene::fx::TexFX.GetEffect(), scene::fx::TexFX.GetTechnique());
	scene::light_sources::Parallel.Position= XMVectorZero();


	//----- Камера

	scene::objects::camera.Position= XMVectorSet(0, 10, -32, 0);
	//scene::objects::camera.RotateAroundYAxis(Pi+Pi/180*5);
	//scene::objects::camera.RotateAroundXAxis(-Pi/6);



	//------ Ткань	

	scene::objects::cloth.InitFromExistingData(d3d->Device, d3d->DeviceContext, scene::fx::TexFX.GetEffect(), scene::fx::TexFX.GetTechnique());
	
	scene::objects::cloth.ConstructGeometry(10, 10, 1);
	//scene::objects::cloth.ConstructGeometry(40, 40, 1);
	scene::objects::cloth.m= 3;

	scene::objects::cloth.FX11.TextureDiffuseApply("resources\\textures\\cloth.dds");
	scene::objects::cloth.FX11.TextureSpecularApply("resources\\textures\\defaultspec.dds");

	scene::objects::cloth.Position= XMVectorZero();


	//----- Куб для визуализации источника света

	scene::objects::box.InitFromExistingData(d3d->Device, d3d->DeviceContext, scene::fx::TexFX.GetEffect(), scene::fx::TexFX.GetTechnique());

	scene::objects::box.InfoVectorsConstruct(ObjectSpace, NULL);

	scene::objects::box.ConstructGeometry();
	scene::objects::box.Position= XMVectorSet(0, 0, 10, 0);


	//------ Инфовекторы

	scene::objects::infoV.Init(d3d->Device, d3d->DeviceContext, "InfoVectors.fx", "InfoTech");

	vector<VertexPosColor> v;
	v.push_back(VertexPosColor(scene::objects::box.Position, RED)); //0
	v.push_back(VertexPosColor(scene::objects::box.Right, RED)); //1

	v.push_back(VertexPosColor(scene::objects::box.Position, GREEN)); //2
	v.push_back(VertexPosColor(scene::objects::box.Up, GREEN)); //3

	v.push_back(VertexPosColor(scene::objects::box.Position, BLUE)); //4
	v.push_back(VertexPosColor(scene::objects::box.Direction, BLUE)); //5

	vector<DWORD> indcs;
	indcs.push_back(0);
	indcs.push_back(1);
	indcs.push_back(2);
	indcs.push_back(3);
	indcs.push_back(4);
	indcs.push_back(5);
	
	
	scene::objects::infoV.Construct(v, indcs);


	
	




  //HRESULT hr =	D3DX11CreateFont(d3d->Device, 20, 0, FW_BLACK, 8, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
	//	                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &pFont); // recommended to use DirectWrite and the DirectXTK library, SpriteFont class instead

}


void Application::UpdateScene(float dt)
{

	D3DApplication::UpdateScene(dt);



	//Камера

	scene::objects::camera.Observe(hWnd, aim.x, aim.y, ClientWidth/2, ClientHeight/2, 1e-3*1.8);


	float p = 1e2/5; // speed factor 

	if(GetAsyncKeyState('W') & 0x8000) scene::objects::camera.TranslateZ( 10e-3*p);
	if(GetAsyncKeyState('S') & 0x8000) scene::objects::camera.TranslateZ(-10e-3*p);
	if(GetAsyncKeyState('A') & 0x8000) scene::objects::camera.TranslateX(-10e-3*p);
	if(GetAsyncKeyState('D') & 0x8000) scene::objects::camera.TranslateX( 10e-3*p);
	

	p= Pi/180*1e-8;

	if(GetAsyncKeyState(VK_UP) & 0x8000)	
	{
			//forceY += p;
			scene::objects::plane.RotateAroundXAxis(p);

	}

	if(GetAsyncKeyState(VK_DOWN) & 0x8000)	
	{
			
			//forceY-= p;
			scene::objects::plane.RotateAroundXAxis(-p);

	}


	
	p= 1e1;

	if(GetAsyncKeyState(VK_NUMPAD8) & 0x8000)	scene::objects::sphere.TranslateZ(1e-2*p);
	if(GetAsyncKeyState(VK_NUMPAD2) & 0x8000)	scene::objects::sphere.TranslateZ(-1e-2*p);
	if(GetAsyncKeyState(VK_NUMPAD4) & 0x8000)	scene::objects::sphere.TranslateX(-1e-2*p);
	if(GetAsyncKeyState(VK_NUMPAD6) & 0x8000)	scene::objects::sphere.TranslateX(1e-2*p);	

	p= 1e1*2;

	if(GetAsyncKeyState(VK_NUMPAD7) & 0x8000)	scene::objects::sphere.TranslateY(1e-2*p);

	p= 1e0;

	if(GetAsyncKeyState(VK_NUMPAD9) & 0x8000)	scene::objects::sphere.TranslateY(-1e-2*p);

	if(GetAsyncKeyState(VK_NUMPAD5) & 0x8000)	
	{
			scene::objects::sphere.SetPosition(0, h, 0);
			scene::objects::sphere.SetVelocity(XMVectorZero(), 1);
			scene::objects::sphere.Force = XMVectorZero();
	}


	//Изменение массы сферы
	p= 1e-2;

	if(GetAsyncKeyState(VK_ADD) & 0x8000)	scene::objects::sphere.m += p;
	if(GetAsyncKeyState(VK_SUBTRACT) & 0x8000)	scene::objects::sphere.m -= p;


	
	//Изменение жёсткости ткани (пружин)
	p= 1e-2*5;
	
	if(GetAsyncKeyState(VK_NUMPAD1) & 0x8000)	    scene::objects::cloth.Kd -= p;
	if(GetAsyncKeyState(VK_NUMPAD3) & 0x8000)	scene::objects::cloth.Kd += p;

	// Вкл\Выкл реакцию сферы на столкновение с тканью
	if(GetAsyncKeyState(VK_DIVIDE) & 0x8000)   
	{
			isBounce = !isBounce;
	}


	// Вкл\Выкл действие гравитации на сферу
	if(GetAsyncKeyState(VK_MULTIPLY) & 0x8000) 
	{
			isApplyGravity = !isApplyGravity;
	}



	p= 1e-2*2;


	if(GetAsyncKeyState('G') & 0x8000) theta-= p;
	if(GetAsyncKeyState('J') & 0x8000) theta+= p;	

	if(GetAsyncKeyState('Y') & 0x8000) phi-= p;
	if(GetAsyncKeyState('H') & 0x8000) phi+= p;	


	p= 1e-2;

	if(GetAsyncKeyState(VK_UP) & 0x8000)	radius+= p;
	if(GetAsyncKeyState(VK_DOWN) & 0x8000)	radius-= p;

	
	
	EyePos.m128_f32[0] =  radius*sinf(phi)*sinf(theta);
	EyePos.m128_f32[2] = -radius*sinf(phi)*cosf(theta);
	EyePos.m128_f32[1] =  radius*cosf(phi);
	
	/*
	EyePos.m128_f32[0] =  radius*sinf(phi)*sinf(theta)+scene::objects::plane2.Position.m128_f32[0];
	EyePos.m128_f32[2] = -radius*sinf(phi)*cosf(theta)+scene::objects::plane2.Position.m128_f32[2];
	EyePos.m128_f32[1] =  radius*cosf(phi)+scene::objects::plane2.Position.m128_f32[1];
	*/
	scene::objects::box.Position= EyePos;


	mLookAt= XMMatrixLookAtLH(EyePos,
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	

	
}


void Application::OnResize()
{

	D3DApplication::OnResize();

	float aspectratio = (float)ClientWidth/ClientHeight;

	Proj= XMMatrixPerspectiveFovLH(0.25*Pi, aspectratio, 0.5f, 1e3f);

}


void Application::RenderScene()
{

	D3DApplication::RenderScene();

	


	//
	// Установка параметров сцены
	//


		View= scene::objects::camera.ObjectSpace;
		
		WVP = View*Proj;


//-----------------Отрисовк сцены с использованием ShaderSystem---------------------------

	//---------Объекты отражатели для обновления константных буферов

		VS_cbPerScene vs_cbPerScene;

		vs_cbPerScene.WV= View;
		vs_cbPerScene.View= View;
		vs_cbPerScene.LightWVP= scene::shadow_mapping::LightWVP;


		VS_cbPerObject vs_cbPerObject;

		vs_cbPerObject.LWVP= WVP;
		vs_cbPerObject.World= XMMatrixIdentity();
		vs_cbPerObject.TexMtx= XMMatrixIdentity();		



		PS_cbPerFrame ps_cbPerFrame;

		ps_cbPerFrame.WV= View;
		ps_cbPerFrame.LightDir= scene::light_sources::Parallel.Direction;
		ps_cbPerFrame.LightPos= scene::light_sources::Parallel.Position;
		ps_cbPerFrame.pos_test2= scene::light_sources::Parallel.Position;
		ps_cbPerFrame.light= scene::light_sources::Parallel;
		ps_cbPerFrame.light2= scene::light_sources::Parallel;
		ps_cbPerFrame.EyePosW= scene::objects::camera.Position;
		ps_cbPerFrame.L_ambient= scene::light_sources::Parallel.ambient;
		ps_cbPerFrame.L_diffuse= scene::light_sources::Parallel.diffuse;
		ps_cbPerFrame.L_specular= scene::light_sources::Parallel.specular;
		ps_cbPerFrame.LightSourceType= scene::light_sources::Parallel.LSType;


		

	
	//---------Обновление КБ

		*scene::objects::sphere2.FX["RenderFX"].shaders["VS"].constant_buffers["vs_cbPerScene"]= &vs_cbPerScene;
		*scene::objects::sphere2.FX["RenderFX"].shaders["VS"].constant_buffers["vs_cbPerObject"]= &vs_cbPerObject;

		*scene::objects::sphere2.FX["RenderFX"].shaders["PS"].constant_buffers["ps_cbPerFrame"]= &ps_cbPerFrame;


		*scene::objects::plane2.FX["RenderFX"].shaders["VS"].constant_buffers["vs_cbPerScene"]= &vs_cbPerScene;
		*scene::objects::plane2.FX["RenderFX"].shaders["VS"].constant_buffers["vs_cbPerObject"]= &vs_cbPerObject;

		*scene::objects::plane2.FX["RenderFX"].shaders["PS"].constant_buffers["ps_cbPerFrame"]= &ps_cbPerFrame;

		
		
		VS_build_depthmap_cbPerFrame vs_build_depthmap_cbPerFrame;
		vs_build_depthmap_cbPerFrame.LightWVP= scene::light_sources::Parallel.GetTransformToObjectSpace()*scene::shadow_mapping::LightVolumeProj;

		*scene::objects::sphere2.FX["DepthMapBuildFX"].shaders["VS"].constant_buffers["vs_build_depthmap_cbPerFrame"]= &vs_build_depthmap_cbPerFrame;
		*scene::objects::plane2.FX["DepthMapBuildFX"].shaders["VS"].constant_buffers["vs_build_depthmap_cbPerFrame"]= &vs_build_depthmap_cbPerFrame;




	//---------Отрисовка сцены

		scene::objects::sphere2.Render(WVP, fill_mode, 1, D3D11_CULL_NONE);
		scene::objects::plane2.Render(WVP, fill_mode, 1, D3D11_CULL_NONE);



//----------------------------------------------------------------------------------------

	

		scene::light_sources::Parallel.Position= scene::objects::box.Position;
//		scene::light_sources::Parallel.Direction= -scene::objects::box.Position+scene::objects::plane2.Position;
		scene::light_sources::Parallel.Direction= -scene::objects::box.Position;
		
		scene::light_sources::Parallel.Cast(&scene::objects::camera.Position);

		XMVECTOR lightPos;
		
		//lightPos = TransformVec3AsPoint(scene::light_sources::Parallel.Position, View);
		//scene::fx::TexFX.GetEffect()->GetVariableByName("pos_test2")->SetRawValue(&lightPos, 0, sizeof(XMVECTOR));


		struct LS
		{
			//XMMATRIX basis;
	
			XMVECTOR pos;
			//float pad1;

			XMVECTOR right;
			//float pad2;

			XMVECTOR up;
			//float pad3;

			XMVECTOR dir;		
			//float pad4;

			//XMVECTOR changed;
			//float pad5;

			//float4x4 basis;
			XMVECTOR ambient;
			XMVECTOR diffuse;
			XMVECTOR specular;
	
			XMVECTOR att;
			//float pad6;

			float  spotPower;
			float  range;
			float theta; // Max cone angle
			int LSType;
		};

		LS ls;

		//ls.basis= scene::light_sources::Parallel.ObjectSpace;
		ls.pos= scene::light_sources::Parallel.fPosition;
		ls.right= scene::light_sources::Parallel.fRight;
		ls.up= scene::light_sources::Parallel.fUp;
		ls.dir= scene::light_sources::Parallel.fDirection;
		//ls.changed= scene::light_sources::Parallel.changed;
		ls.ambient= scene::light_sources::Parallel.ambient;
		ls.diffuse= scene::light_sources::Parallel.diffuse;
		ls.specular= scene::light_sources::Parallel.specular;
		ls.att= scene::light_sources::Parallel.att;
		ls.spotPower= scene::light_sources::Parallel.spotPow;
		ls.range= scene::light_sources::Parallel.range;
		ls.theta= scene::light_sources::Parallel.ConeMaxAngle;
		ls.LSType= scene::light_sources::Parallel.LSType;



		//scene::fx::TexFX.GetEffect()->GetVariableByName("light")->SetRawValue(&ls, 0, sizeof(ls));


		//scene::fx::TexFX.GetEffect()->GetVariableByName("LightDir")->SetRawValue(&XMVectorSet(scene::light_sources::Parallel.Direction, 0), 0, sizeof(XMVECTOR));
		scene::fx::TexFX.GetEffect()->GetVariableByName("LightPos")->SetRawValue(&scene::light_sources::Parallel.Position, 0, sizeof(XMVECTOR));

		//scene::fx::TexFX.GetEffect()->GetVariableByName("LightDir")->AsVector()->SetFloatVector( (float*) &XMVectorSet(scene::light_sources::Parallel.Direction, 0) );

		fxLightDir= scene::fx::TexFX.GetEffect()->GetVariableByName("LightDir")->AsVector();
		fxLightDir->SetFloatVector( (float*) &scene::light_sources::Parallel.Direction );

		scene::fx::TexFX.GetEffect()->GetVariableByName("L_ambient")->SetRawValue(&scene::light_sources::Parallel.ambient, 0, sizeof(XMVECTOR));
		scene::fx::TexFX.GetEffect()->GetVariableByName("L_diffuse")->SetRawValue(&scene::light_sources::Parallel.diffuse, 0, sizeof(XMVECTOR));
		scene::fx::TexFX.GetEffect()->GetVariableByName("L_specular")->SetRawValue(&scene::light_sources::Parallel.specular, 0, sizeof(XMVECTOR));


		scene::objects::sphere.WV= View; // передаётся для формирования матрицы WV
		scene::objects::box.WV= View;
		scene::objects::cloth.WV= View;



		XMVECTOR Force = XMVectorSet(0, 0, 0, 0);

		 isBounce = false;
		 //isApplyGravity = false;
		 isApplyGravity = true;

		 //float dt = mTimer.getDeltaTime(); 

		 //float dt = 1e-2*2.5;
		 float dt = 1;


		 //
		ID3DX11EffectTechnique * TexTech = scene::fx::TexFX.GetTechnique(), 
							  * DepthMapCreateTech = scene::fx::ShadowFX.GetTechnique();



	//
	// Отрисовка сцены, физическая симуляция
	//
		//---Shadow mapping

		////scene::shadow_mapping::depthMap->Set_DSV_without_RTV_Set_VP(); // привязка текстуры через DSV к OM-стадии конвейера для получения карты глубин

			// глубины отмеряются по отношению к ИС => нужно перейти к ВП ИС	
			scene::shadow_mapping::LightWVP= scene::light_sources::Parallel.GetTransformToObjectSpace()*scene::shadow_mapping::LightVolumeProj;
			//scene::shadow_mapping::LightWVP= mLookAt*scene::shadow_mapping::LightVolumeProj;
		

			
			// для создания карты глубин используется отдельная техника
			scene::objects::sphere.FX11.SetTechnique(DepthMapCreateTech);
			scene::objects::cloth.FX11.SetTechnique(DepthMapCreateTech);
			

			/// Отрисовка сцены в depth map

				
			    // передача матрицы LightWVP в эффект build_depthmap.fx
				scene::fx::ShadowFX.GetEffect()->GetVariableByName("LightWVP")->AsMatrix()->SetMatrix((float*)&scene::shadow_mapping::LightWVP);
			   
				
	////		scene::objects::sphere.Render(WVP, D3D11_FILL_SOLID, 1, D3D11_CULL_NONE, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, StateAfterRender::obj_changed);

				//scene::fx::ShadowFX.GetEffect()->GetVariableByName("LightWVP")->AsMatrix()->SetMatrix((float*)&scene::shadow_mapping::LightWVP);
			   
				
	////		scene::objects::cloth.Render(WVP, D3D11_FILL_SOLID, 1, D3D11_CULL_NONE, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, StateAfterRender::obj_changed);

		//----

		scene::shadow_mapping::depthMap->GenerateMipsForSRV();
		

		////Reset_RTV_DSV_VP(ClientWidth, ClientHeight); // восстановление прежних RTV, DSV и VP

		


		scene::objects::sphere.FX11.SetTechnique(TexTech);
		scene::objects::cloth.FX11.SetTechnique(TexTech);

		

		 scene::objects::box.Render(WVP, D3D11_FILL_WIREFRAME, 1, D3D11_CULL_NONE);

			 vector<VertexPosColor> v;
			 XMVECTOR x, y, z;
			/*
			 x= scene::objects::box.Position+scene::objects::box.Right;
				 v.push_back(VertexPosColor(scene::objects::box.Position, RED));
				 v.push_back(VertexPosColor(x, RED));

			 y= scene::objects::box.Position+scene::objects::box.Up;
				 v.push_back(VertexPosColor(scene::objects::box.Position, GREEN));
				 v.push_back(VertexPosColor(y, GREEN));

			 z= scene::objects::box.Position+scene::objects::box.Direction;
				 v.push_back(VertexPosColor(scene::objects::box.Position, BLUE));
				 v.push_back(VertexPosColor(z, BLUE));
			*/

/*
			 //--Заполнение контейнера векторами базиса ИС
			 x= scene::light_sources::Parallel.Position+scene::light_sources::Parallel.Right;
				 v.push_back(VertexPosColor(scene::light_sources::Parallel.Position, RED));
				 v.push_back(VertexPosColor(x, RED));

			 y= scene::light_sources::Parallel.Position+scene::light_sources::Parallel.Up;
				 v.push_back(VertexPosColor(scene::light_sources::Parallel.Position, GREEN));
				 v.push_back(VertexPosColor(y, GREEN));

			 z= scene::light_sources::Parallel.Position+scene::light_sources::Parallel.Direction;
				 v.push_back(VertexPosColor(scene::light_sources::Parallel.Position, BLUE));
				 v.push_back(VertexPosColor(z, BLUE));
*/

			
			 
			 //Создание геометрии инфовекторов - базис ИС
			 x= scene::objects::box.Position+XMVectorSet(mLookAt.r[0].m128_f32[0], mLookAt.r[1].m128_f32[0], mLookAt.r[2].m128_f32[0], 0);

				 v.push_back(VertexPosColor(scene::objects::box.Position, RED));
				 v.push_back(VertexPosColor(x, RED));

			 y= scene::objects::box.Position+XMVectorSet(mLookAt.r[0].m128_f32[1], mLookAt.r[1].m128_f32[1], mLookAt.r[2].m128_f32[1], 0);

				 v.push_back(VertexPosColor(scene::objects::box.Position, GREEN));
				 v.push_back(VertexPosColor(y, GREEN));

			 z= scene::objects::box.Position+XMVectorSet(mLookAt.r[0].m128_f32[2], mLookAt.r[1].m128_f32[2], mLookAt.r[2].m128_f32[2], 0);

				 v.push_back(VertexPosColor(scene::objects::box.Position, BLUE));
				 v.push_back(VertexPosColor(z, BLUE));
				 

				scene::objects::infoV.SetVertices(v); // установка геометрии инфовекторов

			 scene::objects::infoV.Render(WVP, D3D11_FILL_SOLID, 1, D3D11_CULL_NONE, D3D11_PRIMITIVE_TOPOLOGY_LINELIST); // отрисовка базиса ИС


			 //scene::objects::plane.Render(&DirectX::XMMatrixIdentity(), D3D11_FILL_SOLID, 1, D3D11_CULL_NONE);
			 
			 scene::objects::plane.FX11.TextureDiffuseApply(scene::shadow_mapping::depthMap->Get_SRV());
			 //scene::objects::plane.Render(&WVP, D3D11_FILL_SOLID, 1, D3D11_CULL_NONE);


		CollideClothWithBall(&scene::objects::cloth, &scene::objects::sphere, isApplyGravity, dt);

scene::fx::TexFX.GetEffect()->GetVariableByName("depthMap")->AsShaderResource()->SetResource(scene::shadow_mapping::depthMap->Get_SRV());
scene::fx::TexFX.GetEffect()->GetVariableByName("LightWVP")->AsMatrix()->SetMatrix((float*)&scene::shadow_mapping::LightWVP);

/*
		scene::fx::TexFX.GetEffect()->GetVariableByName("LightPos")->SetRawValue(&scene::light_sources::Parallel.Position, 0, sizeof(XMVECTOR));
		
		scene::fx::TexFX.GetEffect()->GetVariableByName("LightDir")->AsVector()->SetFloatVector( (float*) &scene::light_sources::Parallel.Direction);
		//fxLightDir= scene::fx::TexFX.GetEffect()->GetVariableByName("LightDir")->AsVector();
		//fxLightDir->SetFloatVector( (float*) &scene::light_sources::Parallel.Direction );

		scene::fx::TexFX.GetEffect()->GetVariableByName("L_ambient")->SetRawValue(&scene::light_sources::Parallel.ambient, 0, sizeof(XMVECTOR));
		scene::fx::TexFX.GetEffect()->GetVariableByName("L_diffuse")->SetRawValue(&scene::light_sources::Parallel.diffuse, 0, sizeof(XMVECTOR));
		scene::fx::TexFX.GetEffect()->GetVariableByName("L_specular")->SetRawValue(&scene::light_sources::Parallel.specular, 0, sizeof(XMVECTOR));
*/


	////	scene::objects::sphere.Render(WVP, fill_mode, 1, D3D11_CULL_NONE);

//scene::fx::TexFX.GetEffect()->GetVariableByName("depthMap")->AsShaderResource()->SetResource(scene::shadow_mapping::depthMap->Get_SRV());
//scene::fx::TexFX.GetEffect()->GetVariableByName("LightWVP")->AsMatrix()->SetMatrix((float*)&scene::shadow_mapping::LightWVP);

		//// scene::objects::cloth.Render(WVP, fill_mode, 1, D3D11_CULL_NONE);

		 //CollideClothWithBall(&scene::objects::cloth, &scene::objects::sphere, isApplyGravity, dt);



	//
	// Вывод статистики
	// 


std::stringstream ss;



		stats = "Sphere velocity:\nX: ";
		
		ss << scene::objects::sphere.GetVelocity().m128_f32[0];

		stats += ss.str();

		ss.str("RenderFX");

		ss << scene::objects::sphere.GetVelocity().m128_f32[1];

		stats += "\nY: "+ss.str();

		ss.str("RenderFX");

		ss << scene::objects::sphere.GetVelocity().m128_f32[2];

		stats += "\nZ: "+ss.str();

		ss.str("RenderFX");

		

		

		stats += "\n\nSphere position:\nX: ";
		
		ss << scene::objects::sphere.Position.m128_f32[0];

		stats += ss.str();

		ss.str("RenderFX");

		ss << scene::objects::sphere.Position.m128_f32[1];

		stats += "\nY: "+ss.str();

		ss.str("RenderFX");

		ss << scene::objects::sphere.Position.m128_f32[2];

		stats += "\nZ: "+ss.str();

		
		if (isApplyGravity)
		stats += "\n\nGravity ACTIVE";
		else
		stats += "\n\nGravity NOT ACTIVE";


		
		
		//TextOutput(pFont, stats+ss.str());
		
		//TextOutput(pFont, stats, 300, 180);

		

		

		ss.str("RenderFX");


		ss<<scene::objects::cloth.Kd;

		stats += "\n\nCloth.Kd: " + ss.str();

		ss.str("RenderFX");


		ss<<scene::objects::sphere.Kv;

		stats += "\n\nBall.Kv: " + ss.str();

		ss.str("RenderFX");


		ss<<scene::objects::cloth.Kv;

		stats += "\n\nCloth.Kv: " + ss.str();

		ss.str("RenderFX");


		ss<<scene::objects::sphere.Force.m128_f32[1];
		//ss<<scene::objects::sphere.Force.m128_f32[1];

		stats += "\n\nForce.m128_f32[1]:  "+ss.str();

		ss.str("RenderFX"); 


		ss<<scene::objects::sphere.Force_total.m128_f32[1];
		//ss<<scene::objects::sphere.Force.m128_f32[1];

		stats += "\n\nForce_total.m128_f32[1]:  "+ss.str();


		
		ss.str("RenderFX");

		stats += "\n\n\n\n\nCamera position:\nX: ";
		
		ss << scene::objects::camera.Position.m128_f32[0];

		stats += ss.str();

		ss.str("RenderFX");

		ss << scene::objects::camera.Position.m128_f32[1];

		stats += "\nY: "+ss.str();

		ss.str("RenderFX");

		ss << scene::objects::camera.Position.m128_f32[2];

		stats += "\nZ: "+ss.str();



		ss.str("RenderFX");

/*
		if(mLookAt == scene::light_sources::Parallel.GetTransformToObjectSpace())
		{
			ss<<true;
			stats+= "\n\nequal"+ss.str();
		}
		else 
			{
				ss<<false;
				stats+= "\n\nnotequal"+ss.str();
			}
*/

		XMVECTOR s_pos = XMVector3Transform(scene::objects::sphere.Position, scene::light_sources::Parallel.GetTransformToObjectSpace());

		ss<<"Sphere's pos in LS local space:\n\n";
		ss<<"x: "<<s_pos.m128_f32[0]<<"  y: "<<s_pos.m128_f32[1]<<"  z: "<<s_pos.m128_f32[2];

		stats+= ss.str();



				//TextOutput(pFont, stats);

		
		

	d3d->SwapChain->Present(0, 0); 	 

}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	

	Application d3dapplication;


	if(!d3dapplication.Init(hInstance, SW_SHOWMAXIMIZED)) return 0;

			d3dapplication.InitScene();	
				
					return  d3dapplication.MessageLoop();

	
}


LRESULT Application::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{

	POINT mousePos;
	int dx = 0;
	int dy = 0;

	switch(msg)
	{

		case WM_LBUTTONDOWN:
		if( wParam & MK_LBUTTON )
		{
			SetCapture(hWnd);

			mOldMousePos.x = LOWORD(lParam);
			mOldMousePos.y = HIWORD(lParam);
		}
		return 0;

	case WM_LBUTTONUP:
		ReleaseCapture();
		return 0;	

	case WM_MOUSEMOVE:

		{
			aim.x= GET_X_LPARAM(lParam);
			aim.y= GET_Y_LPARAM(lParam);
			

			if( wParam & MK_LBUTTON )
			{

				mousePos.x = (int)LOWORD(lParam); 
				mousePos.y = (int)HIWORD(lParam); 

				dx = mousePos.x - mOldMousePos.x;
				dy = mousePos.y - mOldMousePos.y;
						 

				mOldMousePos = mousePos;

			}

	    return 0;
		
		
		}

		case WM_KEYDOWN:

			switch(wParam)
			{

				case VK_ESCAPE: DestroyWindow(hWnd);
					  
					             return 0;

				case VK_SPACE:
					
					switch(fill_mode)		
					{
						
						case D3D11_FILL_SOLID: fill_mode= D3D11_FILL_WIREFRAME;
									
							     return 0;

						case D3D11_FILL_WIREFRAME: fill_mode= D3D11_FILL_SOLID;												   

					}

							return 0;
					           
					             
			}


		
		return 0;


		default: 
			return D3DApplication::WndProc(msg, wParam, lParam);	

	}


}

/*
void Application::TextOutput(ID3DX10Font* const font, const string& text, const DWORD& left, const DWORD& top)
{


	RECT r;
	SetRectEmpty(&r);


	r.left = left;
	r.top = top;


	font->DrawTextA(NULL, text.c_str(), -1, &r, DT_LEFT | DT_CALCRECT, XMVectorSet(1, 0, 0, 1));

	
	font->DrawTextA(NULL, text.c_str(), -1, &r, DT_LEFT | DT_WORDBREAK | DT_NOCLIP, XMVectorSet(51.0/255, 131.0/255, 219.0/255, 1));	
			
	//font->DrawTextA(NULL, text.c_str(), -1, &r, DT_LEFT | DT_WORDBREAK | DT_NOCLIP, XMVectorSet(1, 0.5, 0, 1));	

	//font->DrawTextA(NULL, text.c_str(), -1, &r, DT_LEFT | DT_WORDBREAK | DT_NOCLIP, XMVectorSet(0, 0, 1, 1));	


}
*/