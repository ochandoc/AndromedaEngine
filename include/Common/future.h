#pragma once

#include "base.h"

namespace And
{
	namespace internal
	{
		template<typename, typename...>
		class job_imp;

		class availability_base
		{
		public:
			availability_base() = default;
			virtual ~availability_base() = default;

			virtual bool is_available() const = 0;
		};

		template<typename T>
		class future_base : public availability_base
		{
		public:
			future_base() = default;
			virtual ~future_base() = default;

			virtual T get() = 0;
			virtual void wait() const = 0;
		};

		template<typename T>
		class future_imp : public future_base<T>
		{
		public:
			future_imp(std::future<T>& f) { m_Future = std::move(f); }
			virtual ~future_imp() = default;

			virtual T get() override
			{
				return m_Future.get();
			}

			virtual bool is_available() const override
			{
				return (m_Future.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready);
			}

			virtual void wait() const override
			{
				m_Future.wait();
			}

		private:
			std::future<T> m_Future;
		};

		template<typename T>
		class future_fake : public future_base<T>
		{
		public:
			future_fake(T value) : m_Value(value) {}
			virtual ~future_fake() = default;

			virtual T get() override
			{
				return m_Value;
			}

			virtual bool is_available() const override
			{
				return true;
			}

			virtual void wait() const override {}

		private:
			T m_Value;
		};
	}
	template<typename T>
	class future
	{
	private:
		future() = default;

	public:
		future(T value) { m_Future = std::make_shared<internal::future_fake<T>>(value); m_Id = (size_t)m_Future.get(); }
		future(const future& other) { m_Future = other.m_Future; m_Id = other.m_Id; }
		future(future&& other) { m_Future = std::move(other.m_Future); std::swap(m_Id, other.m_Id); }

		~future() = default;

		future& operator =(const future& other) { if (this != &other) { m_Future = other.m_Future; m_Id = other.m_Id; } return *this; }
		future& operator =(future&& other) { if (this != &other) { m_Future = std::move(other.m_Future); std::swap(m_Id, other.m_Id); } return *this; }

		bool operator ==(const future& other) { return (m_Id == other.m_Id); }

		T get()
		{
			return m_Future->get();
		}

		bool is_available() const
		{
			return m_Future->is_available();
		}

		void wait() const
		{
			m_Future->wait();
		}

		size_t get_id() const { return m_Id; }

		template<typename, typename...>
		friend class internal::job_imp;
		friend class future_availability;
	private:
		std::shared_ptr<internal::future_base<T>> m_Future;
		size_t m_Id;
	};

	template<>
	class future<void>
	{
	private:
		future() = default;

	public:
		future(const future& other) { m_Future = other.m_Future; m_Id = other.m_Id; }
		future(future&& other) { m_Future = std::move(other.m_Future); std::swap(m_Id, other.m_Id); }

		~future() = default;

		future& operator =(const future& other) { if (this != &other) { m_Future = other.m_Future; m_Id = other.m_Id; } return *this; }
		future& operator =(future&& other) { if (this != &other) { m_Future = std::move(other.m_Future); std::swap(m_Id, other.m_Id); } return *this; }

		bool operator ==(const future& other) { return (m_Id == other.m_Id); }

		void get()
		{
			return m_Future->get();
		}

		bool is_available() const
		{
			return m_Future->is_available();
		}

		void wait() const
		{
			m_Future->wait();
		}

		size_t get_id() const { return m_Id; }

		template<typename, typename...>
		friend class internal::job_imp;
		friend class future_availability;
	private:
		std::shared_ptr<internal::future_base<void>> m_Future;
		size_t m_Id;
	};

	class future_availability
	{
	public:
		template<typename T>
		future_availability(future<T>& f) { m_Future = f.m_Future; m_Id = f.m_Id; }
		future_availability(const future_availability& other) { m_Future = other.m_Future; m_Id = other.m_Id; }
		future_availability(future_availability&& other) { m_Future = std::move(other.m_Future); std::swap(m_Id, other.m_Id); }

		~future_availability() = default;

		future_availability& operator=(const future_availability& other) { if (this != &other) { m_Future = other.m_Future; m_Id = other.m_Id; } return *this; }
		future_availability& operator=(future_availability&& other) { if (this != &other) { m_Future = std::move(other.m_Future); std::swap(m_Id, other.m_Id); } return *this; }

		bool is_available() const
		{
			return m_Future->is_available();
		}

		size_t get_id() const
		{
			return m_Id;
		}

	private:
		std::shared_ptr<internal::availability_base> m_Future;
		size_t m_Id;
	};
}