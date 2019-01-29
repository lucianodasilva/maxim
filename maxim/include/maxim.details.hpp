#pragma once
#ifndef _maxim_details_hpp_
#define _maxim_details_hpp_

namespace maxim {
	
	namespace details {

		template < typename _derived_t, typename _value_t >
		struct value_base
		{
		public:

			using value_type = _value_t;

			inline value_base () = default;

			inline value_base (std::initializer_list < value_type > args ){}

			inline _derived_t operator + (_derived_t const & v) const {
				return apply(
					v,
					[](auto v1, auto v2) { return v1 + v2; }
				);
			}

			inline _derived_t operator - (_derived_t const & v) const {
				return apply(
					v,
					[](auto v1, auto v2) { return v1 - v2; }
				);
			}

			inline _derived_t operator * (_derived_t const & v) const {
				return apply(
					v,
					[](auto v1, auto v2) { return v1 * v2; }
				);
			}

			inline _derived_t operator * (value_type const & v) const {
				return apply_scalar(
					v,
					[](auto v1, auto v2) { return v1 * v2; }
				);
			}

			inline _derived_t operator / (_derived_t const & v) const {
				return apply(
					v,
					[](auto v1, auto v2) { return v1 / v2; }
				);
			}

			inline _derived_t operator / (value_type const & v) const {
				return apply_scalar(
					v,
					[](auto v1, auto v2) { return v1 / v2; }
				);
			}

			inline bool operator == (_derived_t const & v) const {
				return apply_reduce(
					true,
					v,
					[](bool r, auto v1, auto v2) { return r && (v1 == v2); }
				);
			}

			inline bool operator != (_derived_t const & v) const {
				return !operator==(v);
			}

			inline bool is_zero() const {
				return operator == ({});
			}

		private:

			template < typename _op_t >
			inline _derived_t apply(_derived_t const & v, _op_t && op) const {
				_derived_t r = {};

				auto & _this = *static_cast <_derived_t const *> (this);

				for (int i = 0; i < _derived_t::data_length; ++i)
					r.data[i] = op(_this.data[i], v.data[i]);

				return r;
			}

			template < typename _op_t >
			inline _derived_t apply_scalar(value_type const & v, _op_t && op) const {
				_derived_t r = {};

				auto & _this = *static_cast <_derived_t const *> (this);

				for (int i = 0; i < _derived_t::data_length; ++i)
					r.data[i] = op(_this.data[i], v);

				return r;
			}

			template < typename _r_t, typename _op_t >
			inline auto apply_reduce(_r_t zero, _derived_t const & v, _op_t && op) const {
				_r_t r = zero;

				auto & _this = *static_cast <_derived_t const *> (this);

				for (int i = 0; i < _derived_t::data_length; ++i)
					r = op(r, _this.data[i], v.data[i]);

				return r;
			}
		};

		template < typename _value_t >
		struct value_2d : public value_base < value_2d < _value_t >, _value_t > {
			static constexpr std::size_t data_length = 2;

			using value_base < value_2d < _value_t >, _value_t >::value_base;

			union {
				_value_t data[data_length];

				_value_t
					x, y;
			};
		};

		template < typename _value_t >
		struct value : public value_base < value < _value_t >, _value_t > {
			static constexpr std::size_t data_length = 1;

			using value_base < value < _value_t >, _value_t >::value_base;

			union {
				_value_t data[data_length];

				_value_t
					v;
			};

			_value_t const & get() {
				return v;
			}

			void set(_value_t const & value) {
				v = value;
			}

		};

	}

}

#endif
