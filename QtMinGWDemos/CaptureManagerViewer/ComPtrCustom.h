#pragma once

#include <unknwnbase.h>

	
	template <typename T>
	class CComPtrCustom
	{
	public:

		CComPtrCustom(T *aPtrElement)
			:element(aPtrElement)
		{
		}

		CComPtrCustom()
			:element(nullptr)
		{
		}

		virtual ~CComPtrCustom()
		{
			Release();
		}

		T* Detach()
		{
			auto lOutPtr = element;

			element = nullptr;

			return lOutPtr;
		}

		T* detach()
		{
			return Detach();
		}

		void Release()
		{
			if (element == nullptr)
				return;

			auto k = element->Release();

			element = nullptr;
		}

		CComPtrCustom& operator = (T *pElement)
		{
			Release();

			if (pElement == nullptr)
				return *this;

			auto k = pElement->AddRef();

			element = pElement;

			return *this;
		}

		void Swap(CComPtrCustom& other)
		{
			T* pTemp = element;
			element = other.element;
			other.element = pTemp;
		}

		T* operator->()
		{
			return element;
		}

		operator T*()
		{
			return element;
		}

		operator T*() const
		{
			return element;
		}

		
		T* get()
		{
			return element;
		}

		T* get() const
		{
			return element;
		}

		T** operator &()
		{
			return &element;
		}

		bool operator !()const
		{
			return element == nullptr;
		}

		operator bool()const
		{
			return element != nullptr;
		}

		bool operator == (const T *pElement)const
		{
			return element == pElement;
		}


		CComPtrCustom(const CComPtrCustom& aCComPtrCustom)
		{
 			if (aCComPtrCustom.operator!())
			{
				element = nullptr;

				return;
			}

			element = aCComPtrCustom;

			auto h = element->AddRef();

			h++;
		}

		CComPtrCustom& operator = (const CComPtrCustom& aCComPtrCustom)
		{
			Release();

			element = aCComPtrCustom;

            if(element != nullptr)
                element->AddRef();

			return *this;
		}

        HRESULT CopyTo(T** ppT) throw()
		{
			if (ppT == NULL)
				return E_POINTER;

			*ppT = element;

			if (element)
				element->AddRef();

			return S_OK;
		}

		HRESULT CoCreateInstance(const CLSID aCLSID)
		{
			T* lPtrTemp;

			auto lresult = ::CoCreateInstance(aCLSID, NULL, CLSCTX_INPROC, IID_PPV_ARGS(&lPtrTemp));

			if (SUCCEEDED(lresult))
			{
				if (lPtrTemp != nullptr)
				{
					Release();

					element = lPtrTemp;
				}

			}

			return lresult;
		}

	protected:

		T* element;
	};

	template <class T, const IID* piid = &__uuidof(T)>
	class CComQIPtrCustom :
		public CComPtrCustom<T>
	{
	public:
		CComQIPtrCustom() throw()
		{
		}
        CComQIPtrCustom(decltype(nullptr)) throw()
		{
		}
        CComQIPtrCustom(T* lp) throw() :
			CComPtrCustom<T>(lp)
		{
			}

        CComQIPtrCustom(IUnknown* lp) throw()
		{
			if (lp != NULL)
			{
                if (FAILED(lp->QueryInterface(*piid, (void **)&this->element)))
                    this->element = NULL;
			}
		}
        T* operator=(decltype(nullptr)) throw()
		{
			CComQIPtrCustom(nullptr).Swap(*this);
			return nullptr;
		}
        T* operator=(T* lp) throw()
		{
			if (*this != lp)
			{
				CComQIPtrCustom(lp).Swap(*this);
			}
			return *this;
		}

        T* operator=(IUnknown* lp) throw()
		{
			if (*this != lp)
			{
                if (FAILED(lp->QueryInterface(*piid, (void **)&this->element)))
                    this->element = NULL;
				else
                    return this->element;
			}
			return *this;
		}
	};

	template<>
	class CComQIPtrCustom<IUnknown, &IID_IUnknown> :
	public CComPtrCustom<IUnknown>
	{
	public:
		CComQIPtrCustom() throw()
		{
		}
        CComQIPtrCustom(IUnknown* lp) throw()
		{
			if (lp != NULL)
			{
				if (FAILED(lp->QueryInterface(__uuidof(IUnknown), (void **)&element)))
					element = NULL;
			}
		}
        CComQIPtrCustom(const CComQIPtrCustom<IUnknown, &IID_IUnknown>& lp) throw()
		{
			this->element = lp.get();
		}

        IUnknown* operator=(const CComQIPtrCustom<IUnknown, &IID_IUnknown>& lp) throw()
		{
			if (this->get() != lp.get())
			{
				this->element = lp.get();

			}
			return *this;
		}
	};

