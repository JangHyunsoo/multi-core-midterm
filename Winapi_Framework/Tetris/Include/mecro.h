#pragma once

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#define SAFE_DELETE(p) if(p) { delete p; p = NULL; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = NULL; }
#define SAFE_RELEASE(p) if(p) {p->Release(); p = NULL; }

#define DECLARE_SINGLE(Type)	\
private:\
	static Type* m_pInst;\
public:\
	static Type* GetInst() {\
		if (m_pInst == NULL)\
			m_pInst = new Type;\
		return m_pInst;\
	}\
	static void DestroyInst() {\
		SAFE_DELETE(m_pInst);\
	}\
private:\
	Type();\
	~Type();

#define DEFINITION_SINGLE(Type) Type* Type::m_pInst = nullptr;
#define GET_SINGE(Type) Type::GetInst()
#define DESTROY_SINGE(Type) Type::DestroyInst();

#define GETRESOLUTION CCore::GetInst()->GetResolution()