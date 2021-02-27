#pragma once

#include <map>
#include <string>
#include <variant>
#include <vector>

namespace twinkle
{
	namespace json
	{
		class data
		{
		public:

			using string_t = std::string;
			using array_t = std::vector<data>;
			using object_t = std::map<string_t, data>;

			enum class Type
			{
				Array,
				Boolean,
				Null,
				Number,
				Object,
				String
			};

			data()
				: m_type(Type::Null)
				, m_numberType(NumberType::Undefined)
				, m_value()
			{}

			data(const bool value)
				: m_type(Type::Boolean)
				, m_numberType(NumberType::Undefined)
				, m_value(value)
			{}

			data(const int value)
				: m_type(Type::Number)
				, m_numberType(NumberType::Int)
				, m_value(value)
			{}

			data(const double value)
				: m_type(Type::Number)
				, m_numberType(NumberType::Double)
				, m_value(value)
			{}

			data(const float value)
				: m_type(Type::Number)
				, m_numberType(NumberType::Float)
				, m_value(value)
			{}

			data(const char* value)
				: m_type(Type::String)
				, m_numberType(NumberType::Undefined)
				, m_value(std::string(value))
			{}

			data(const string_t& value)
				: m_type(Type::String)
				, m_numberType(NumberType::Undefined)
				, m_value(value)
			{}

			data(const array_t& value)
				: m_type(Type::Array)
				, m_numberType(NumberType::Undefined)
				, m_value(value)
			{}

			data(const object_t& value)
				: m_type(Type::Object)
				, m_numberType(NumberType::Undefined)
				, m_value(value)
			{}

			bool is_array() const { return m_type == Type::Array; }
			bool is_bool() const { return m_type == Type::Boolean; }
			bool is_null() const { return m_type == Type::Null; }
			bool is_number() const { return m_type == Type::Number; }
			bool is_object() const { return m_type == Type::Object; }
			bool is_string() const { return m_type == Type::String; }

			bool as_bool() const { return std::get<bool>(m_value); }
			int as_int() const { return std::get<int>(m_value); }
			float as_float() const { return std::get<float>(m_value); }
			double as_double() const { return std::get<double>(m_value); }
			const array_t& as_array() const { return std::get<array_t>(m_value); }
			const object_t& ar_object() const { return std::get<object_t>(m_value); }
			const string_t& as_string() const { return std::get<string_t>(m_value); }

			Type type() const { return m_type; }

			bool operator== (const data& other) const
			{
				if (m_type == other.m_type)
				{
					return m_value == other.m_value;
				}
				return false;
			}

			bool operator!= (const data& other) const
			{
				if (m_type == other.m_type)
				{
					return m_value != other.m_value;
				}
				return true;
			}

			data& operator= (const data& other)
			{
				m_type = other.m_type;
				m_numberType = other.m_numberType;
				m_value = other.m_value;

				return *this;
			}

			data& operator= (const bool value)
			{
				m_type = Type::Boolean;
				m_numberType = NumberType::Undefined;
				m_value = value;

				return *this;
			}

			data& operator= (const int value)
			{
				m_type = Type::Number;
				m_numberType = NumberType::Int;
				m_value = value;

				return *this;
			}

			data& operator= (const double value)
			{
				m_type = Type::Number;
				m_numberType = NumberType::Double;
				m_value = value;

				return *this;
			}

			data& operator= (const float value)
			{
				m_type = Type::Number;
				m_numberType = NumberType::Float;
				m_value = value;

				return *this;
			}

			data& operator= (const char* str)
			{
				return *this = std::string(str);
			}

			data& operator= (const std::string& value)
			{
				m_type = Type::String;
				m_numberType = NumberType::Undefined;
				m_value = value;

				return *this;
			}

			data& operator= (const array_t& value)
			{
				m_type = Type::Array;
				m_numberType = NumberType::Undefined;
				m_value = value;

				return *this;
			}

			data& operator= (const object_t& value)
			{
				m_type = Type::Object;
				m_numberType = NumberType::Undefined;
				m_value = value;

				return *this;
			}



		private:

			enum class NumberType
			{
				Undefined,
				Int,
				Float,
				Double
			};

			// value type
			Type m_type;
			NumberType m_numberType;
			// raw value
			std::variant<bool, int, float, double, string_t, array_t, object_t> m_value;
		};
	}
}