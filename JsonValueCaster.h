// Copyright 2017 Yangga
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#pragma once

#include <json/value.h>

#ifdef USE_BOOST_STRING_REF
#include <boost/utility/string_ref.hpp>
#endif

#include <stdint.h>

namespace Json {

	namespace detail {

		template <typename T> static T valueAs(Json::Value const& v) { static_assert(0 > sizeof(T), "invalid type"); }
		template <> int8_t valueAs(Json::Value const& v) { return static_cast<int8_t>(v.asInt()); }
		template <> uint8_t valueAs(Json::Value const& v) { return static_cast<uint8_t>(v.asInt()); }
		template <> int16_t valueAs(Json::Value const& v) { return static_cast<int16_t>(v.asInt()); }
		template <> uint16_t valueAs(Json::Value const& v) { return static_cast<uint16_t>(v.asInt()); }
		template <> Value::Int valueAs(Json::Value const& v) { return v.asInt(); }
		template <> Value::UInt valueAs(Json::Value const& v) { return v.asUInt(); }
		template <> Value::LargestInt valueAs(Json::Value const& v) { return v.asLargestInt(); }
		template <> long valueAs(Json::Value const& v) { return static_cast<long>(v.asLargestInt()); }
		template <> Value::LargestUInt valueAs(Json::Value const& v) { return v.asLargestUInt(); }
		template <> unsigned long valueAs(Json::Value const& v) { return static_cast<unsigned long>(v.asLargestUInt()); }
		template <> float valueAs(Json::Value const& v) { return v.asFloat(); }
		template <> double valueAs(Json::Value const& v) { return v.asDouble(); }
		template <> bool valueAs(Json::Value const& v) { return v.asBool(); }
		template <> std::string valueAs(Json::Value const& v) { return std::move(v.asString()); }
		template <> const char* valueAs(Json::Value const& v) { return v.asCString(); }

#ifdef USE_BOOST_STRING_REF
		template <> boost::string_ref valueAs(Json::Value const& v) {
			const char* txt = v.asCString();
			if (nullptr == txt)
				return boost::string_ref();

			return boost::string_ref(txt, strlen(txt));
		}
#endif

	}	// namespace detail

	class CasterStatic
	{
	public:
		template <typename T>
		inline static T Get(Json::Value const& node, const char* key) {
			if (node.isNull()) {
				throw std::invalid_argument("node is null");
			}

			if (!node.isObject()) {
				throw std::invalid_argument("node is not an object");
			}

			auto& vNodeSub = node[key];
			if (vNodeSub.isNull()) {
				return T();
			}

			return detail::valueAs<T>(vNodeSub);
		}

		template <typename T>
		inline static T Get(Json::Value const& node, Json::ArrayIndex const& idx) {
			if (node.isNull()) {
				throw std::invalid_argument("node is null");
			}

			if (!node.isArray()) {
				throw std::invalid_argument("node is not an array");
			}

			if (node.size() <= idx) {
				throw std::range_error("invalid range");
			}

			return detail::valueAs<T>(node[idx]);
		}

		template <typename T, typename... Args>
		inline static T Get(Json::Value const& node, const char* key1, Args... args) {
			if (node.isNull()) {
				throw std::invalid_argument("node is null");
			}

			if (!node.isObject()) {
				throw std::invalid_argument("node is not an object");
			}

			return Get<T>(node[key1], args...);
		}

		template <typename T, typename... Args>
		inline static T Get(Json::Value const& node, Json::ArrayIndex const& idx, Args... args) {
			if (node.isNull()) {
				throw std::invalid_argument("node is null");
			}

			if (!node.isArray()) {
				throw std::invalid_argument("node is not an array");
			}

			if (node.size() <= idx) {
				throw std::range_error("invalid range");
			}

			return Get<T>(node[idx], args...);
		}
	};

	class Caster
	{
	public:
		explicit Caster(Json::Value const& node)
				: node_(node) {}

		template <typename T, typename KEY_TYPE = const char*, typename... KEY_TYPES>
		inline T get(KEY_TYPE key, KEY_TYPES... elses) const {
			return CasterStatic::Get<T>(node_, key, std::forward<KEY_TYPES>(elses)...);
		}

	private:
		Json::Value const& node_;
	};

	class CasterCoverDef
	{
	public:
		explicit CasterCoverDef(Json::Value const& node)
				: node_(node) {}

		template <typename T, typename KEY_TYPE = const char*, typename... KEY_TYPES>
		inline T get(KEY_TYPE key, T const& defaultValue, KEY_TYPES... elses) const {
            try {
			    return CasterStatic::Get<T>(node_, key, std::forward<KEY_TYPES>(elses)...);
            } catch(...) {
                return defaultValue;
            }
		}

		template <typename T, typename KEY_TYPE = const char*, typename... KEY_TYPES>
		inline T get(KEY_TYPE key, T && defaultValue, KEY_TYPES... elses) const {
            try {
			    return CasterStatic::Get<T>(node_, key, std::forward<KEY_TYPES>(elses)...);
            } catch(...) {
                return defaultValue;
            }
		}

	private:
		Json::Value const& node_;
    };

}	// namespace Json
