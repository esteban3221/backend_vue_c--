#ifndef _BINARY_FILES
#define _BINARY_FILES

#pragma GCC system_header

#ifdef __cplusplus
	#include <iostream>

	#include <utility>
	#include <stdexcept>
	#include <type_traits>

	#include <string>
	#include <fstream>
	#include <ios>
	#include <vector>
	#include <array>
#else
	#error "You must using C++"
#endif

namespace std
{

	enum __type__data__ {
		_Type_Start  = 0x0001,
		_Type_Finish = 0x0000,
		_Type_Int    = 0x00A1,
		_Type_Float  = 0x00A2,
		_Type_Double = 0x00A3,
		_Type_Char   = 0x00B1,
		_Type_NULL   = 0x00C0,
		_Type_Length = 0x00D0
	};

	typedef double __data__double__;
	typedef int __data__int__;
	struct __table__data__ {
		__data__int__    __indx__ = 0x0000;
		__type__data__   __type__ = _Type_NULL;
		__data__double__ __data__ = (__data__double__)0x0000;
	};

	typedef fstream __data__file__management__;
	typedef string __data__string__;
	struct __extern__help__ {
		__data__file__management__ __file__;
		__data__string__           __name__;
		__data__string__           __char__ = "ABCEDFGHIJKLMNOPQRSTUVWXYZ"
											  "abcedfghijklmnopqrstuvwxyz"
											  "~`!@#$%^&*(-_+=){[]}:;'\  "
											  "\"\\<,.>?/|1234567890";
	};

	typedef char __data__char__;
	typedef float __data__float__;
	class __character__selectecting__ {
		private:
			__extern__help__* __pointer__help__;
		public:
			__character__selectecting__(__extern__help__& __parameter__) {
				__pointer__help__ = &__parameter__;
			}
			__data__char__ __get__char__id(__data__int__& __par__int__) {
				return __pointer__help__->__char__[__par__int__ - 0x0001];
			}

			__data__int__ __id__of__char(__data__char__& __char__par__) {
				__data__int__ __return__data__ = 0;
				for(__data__int__ __data__i__ = 0x0000; 
					__data__i__ < __pointer__help__->__char__.length();
					__data__i__++) {
					if(__char__par__ == __pointer__help__->__char__[__data__i__])
						__return__data__ = __data__i__ + 0x0001;
				}

				return __return__data__;
			}
	};

	typedef void __data__void__;
	typedef bool __data__bool__;
	class __binary__file__class {
		private:
			__data__int__ __data__index__ = 0x0001;
			__extern__help__ __helper__data__;
			vector<__table__data__> __data__of__vector__;
			__character__selectecting__* __char__functional__ = 
				new __character__selectecting__(__helper__data__);
		private:
			__data__void__ __close__management() {
				if(__helper__data__.__file__.is_open()) {
					__helper__data__.__file__.close();
				}
			}

			__data__void__ __open__management__(__data__bool__ __wr__) {
				if(__helper__data__.__file__.is_open()) {
					__helper__data__.__file__.close();
				}

				switch(((__data__int__)__wr__) + 1) {
					case 2: {
						__helper__data__.__file__.open(__helper__data__.__name__,
														ios::trunc | ios::out | ios::binary);
					}
					break;
					case 1: {
						__helper__data__.__file__.open(__helper__data__.__name__,
														ios::in | ios::binary);
					}
					break;
				}
			}
		public:
			__binary__file__class(const char* __name__file__) {
				__helper__data__.__name__ = __name__file__;
			}

			__binary__file__class(__data__string__ __name__file__) {
				__helper__data__.__name__ = __name__file__;
			}
		private:
			__data__void__ __register__new__(__data__int__ __length__) {
				__table__data__ __data__register__;
				__data__register__.__indx__ = __data__index__;
				__data__register__.__type__ = _Type_Start;
				__data__register__.__data__ = (__data__double__)__length__;
				__data__of__vector__.push_back(__data__register__);
			}
			__data__void__ __push__new__() {
				__table__data__ __data__push__;
				__data__push__.__indx__ = __data__index__;
				__data__push__.__type__ = _Type_Finish;
				__data__push__.__data__ = _Type_NULL;
				__data__of__vector__.push_back(__data__push__);
				__data__index__ += 0x0001;
			}
		public:
			template<
				typename __data__template>
			__data__void__ adding_number(__data__template __data__input__) {
				if(is_same<__data__template, __data__int__>::value) {
					__register__new__(0x00001);
					__table__data__ __data__integer__;
					__data__integer__.__indx__ = __data__index__;
					__data__integer__.__type__ = _Type_Int;
					__data__integer__.__data__ = (__data__double__)__data__input__;
					__data__of__vector__.push_back(__data__integer__);
					__push__new__();
				} else if(is_same<__data__template, __data__float__>::value) {
					__register__new__(0x00001);
					__table__data__ __data__floats__;
					__data__floats__.__indx__ = __data__index__;
					__data__floats__.__type__ = _Type_Float;
					__data__floats__.__data__ = (__data__double__)__data__input__;
					__data__of__vector__.push_back(__data__floats__);
					__push__new__();
				} else if(is_same<__data__template, __data__double__>::value) {
					__register__new__(0x00001);
					__table__data__ __data__doubles__;
					__data__doubles__.__indx__ = __data__index__;
					__data__doubles__.__type__ = _Type_Double;
					__data__doubles__.__data__ = (__data__double__)__data__input__;
					__data__of__vector__.push_back(__data__doubles__);
					__push__new__();
				} else {
					throw "Can't know parameter name in adding_number";
				}
			}
			__data__void__ adding_string(__data__string__ __par__) {
				__register__new__(__par__.length());
				for(__data__int__ _tp_ = 0x0000; _tp_ < __par__.length(); _tp_++) {
					__table__data__ __data__char__;
					__data__char__.__indx__ = __data__index__;
					__data__char__.__type__ = _Type_Char;
					__data__char__.__data__ = (__data__double__)__char__functional__->__id__of__char(__par__[_tp_]);
					__data__of__vector__.push_back(__data__char__);
				}
				__push__new__();
			}
		public:
			__data__void__ remove(__data__int__ __id__cant__) {
				__data__int__ __id__temp__ = 0x0001;
				vector<__table__data__> __data__of__vector__temp__;
				for(__data__int__ _tp_ = 0x0000; _tp_ < ((__data__int__)__data__of__vector__.size()); _tp_++) {
					if(__data__of__vector__[_tp_].__indx__ != __id__cant__) {
						__data__of__vector__temp__.push_back(__data__of__vector__[_tp_]);
					}
				}
				__data__of__vector__.clear();
				for(__data__int__ _tp_ = 0x0000; _tp_ < ((__data__int__)__data__of__vector__temp__.size()); _tp_++) {
					if(__data__of__vector__temp__[_tp_].__type__ == _Type_Finish) {
						__data__of__vector__temp__[_tp_].__indx__ = __id__temp__;
						__data__of__vector__.push_back(__data__of__vector__temp__[_tp_]);
						__id__temp__ += 0x00001;
					} else {
						__data__of__vector__temp__[_tp_].__indx__ = __id__temp__;
						__data__of__vector__.push_back(__data__of__vector__temp__[_tp_]);
					}
				}
				__data__index__ = __id__temp__;
			}
			__data__void__ clear() {
				__data__of__vector__.clear();
				__data__index__ = 0x00001;
			}
		public:
			__data__void__ save() {
				__open__management__(true);
				__table__data__ __data__length__;
				__data__length__.__indx__ = 0x0000;
				__data__length__.__type__ = _Type_Length;
				__data__length__.__data__ = (__data__double__)((__data__int__)__data__of__vector__.size());
				__helper__data__.__file__.write((__data__char__*)&__data__length__, sizeof(__table__data__));
				for(__data__int__ _tp_ = 0x0000; _tp_ < ((__data__int__)__data__of__vector__.size()); _tp_++) {
					__helper__data__.__file__.write((__data__char__*)&__data__of__vector__[_tp_], sizeof(__table__data__));
				}
				__close__management();
			}
			__data__void__ init() {
				__data__of__vector__.clear();
				__open__management__(false);
				__table__data__ __data__length__;
				__helper__data__.__file__.seekg(0);
				__helper__data__.__file__.read((__data__char__*)&__data__length__, sizeof(__table__data__));
				for(__data__int__ _tp_ = 0x0001; _tp_ < ((__data__int__)__data__length__.__data__); _tp_++) {
					__table__data__ __data__tp__;
					__helper__data__.__file__.seekg(_tp_ * sizeof(__table__data__));
					__helper__data__.__file__.read((__data__char__*)&__data__tp__, sizeof(__table__data__));
					__data__of__vector__.push_back(__data__tp__);
					__data__index__ = (__data__int__)__data__tp__.__indx__ + 0x0001;
				}
				__close__management();
			}
		public:
			template<
				typename __data__template>
			__data__template get_number(__data__int__ __id__get__) {
				if(is_same<__data__template, __data__int__>::value) {
					for(__data__int__ _tp_ = 0x0000; _tp_ < ((__data__int__)__data__of__vector__.size()); _tp_++) {
						if((__data__of__vector__[_tp_].__type__ == _Type_Start) &&
						   (((__data__int__)__data__of__vector__[_tp_].__data__) == 0x0001) &&
						   (__data__of__vector__[_tp_].__indx__ == __id__get__)) {
							return ((__data__int__)__data__of__vector__[_tp_ + 0x00001].__data__);
						}
					}
				} else if(is_same<__data__template, __data__float__>::value) {
					for(__data__int__ _tp_ = 0x0000; _tp_ < ((__data__int__)__data__of__vector__.size()); _tp_++) {
						if((__data__of__vector__[_tp_].__type__ == _Type_Start) &&
						   (((__data__int__)__data__of__vector__[_tp_].__data__) == 0x0001) &&
						   (__data__of__vector__[_tp_].__indx__ == __id__get__)) {
							return ((__data__float__)__data__of__vector__[_tp_ + 0x00001].__data__);
						}
					}
				} else if(is_same<__data__template, __data__double__>::value) {
					for(__data__int__ _tp_ = 0x0000; _tp_ < ((__data__int__)__data__of__vector__.size()); _tp_++) {
						if((__data__of__vector__[_tp_].__type__ == _Type_Start) &&
						   (((__data__int__)__data__of__vector__[_tp_].__data__) == 0x0001) &&
						   (__data__of__vector__[_tp_].__indx__ == __id__get__)) {
							return ((__data__double__)__data__of__vector__[_tp_ + 0x00001].__data__);
						}
					}
				} else {
					throw "Can't know type name in function get_number";
				}
			}
			__data__string__ get_string(__data__int__ __id__par__) {
				__data__string__ __temp_string__ = "";
				for(__data__int__ _tp_ = 0x0000; _tp_ < ((__data__int__)__data__of__vector__.size()); _tp_++) {
					if((__data__of__vector__[_tp_].__indx__ == __id__par__) &&
					   (__data__of__vector__[_tp_].__type__ == _Type_Char)) {
						__data__int__ __temp__ = (__data__int__)__data__of__vector__[_tp_].__data__;
						__temp_string__ += ((__data__char__)__char__functional__->__get__char__id(__temp__));
					}
				}
				return __temp_string__;
			}
		public:
			__data__string__ type(__data__int__ __id__par__) {
				array<__data__string__, 5> __type__return__ = {
					"int", "float", "double", "string", "unknow"};
				__data__int__ __data__return__select = 0x0004;
				for(__data__int__ _tp_ = 0x0000; _tp_ < ((__data__int__)__data__of__vector__.size()); _tp_++) {
					if((__data__of__vector__[_tp_].__type__ == _Type_Start) &&
					   (__data__of__vector__[_tp_].__indx__ == __id__par__) &&
					   (__data__of__vector__[_tp_].__data__ == 0x00001) &&
					   (__data__of__vector__[_tp_ + 0x00001].__type__ == _Type_Int)) {
						__data__return__select = 0x0000;
					}
					if((__data__of__vector__[_tp_].__type__ == _Type_Start) &&
					   (__data__of__vector__[_tp_].__indx__ == __id__par__) &&
					   (__data__of__vector__[_tp_].__data__ == 0x00001) &&
					   (__data__of__vector__[_tp_ + 0x00001].__type__ == _Type_Float)) {
						__data__return__select = 0x0001;
					}
					if((__data__of__vector__[_tp_].__type__ == _Type_Start) &&
					   (__data__of__vector__[_tp_].__indx__ == __id__par__) &&
					   (__data__of__vector__[_tp_].__data__ == 0x00001) &&
					   (__data__of__vector__[_tp_ + 0x00001].__type__ == _Type_Double)) {
						__data__return__select = 0x0002;
					}
					if((__data__of__vector__[_tp_].__type__ == _Type_Start) &&
					   (__data__of__vector__[_tp_].__indx__ == __id__par__) &&
					   (__data__of__vector__[_tp_].__data__ > 0x00001) &&
					   (__data__of__vector__[_tp_ + 0x00001].__type__ == _Type_Char)) {
						__data__return__select = 0x0003;
					}
				}
				return __type__return__[__data__return__select];
			}
		public:
			__data__int__ length() {
				__data__int__ __data__lengths__ = 0x0000;
				if((__data__int__)__data__of__vector__.size() != 0) {
					__data__lengths__ += 0x00001;
					for(__data__int__ _tp_ = 0x0000; _tp_ < ((__data__int__)__data__of__vector__.size()); _tp_++) {
						if(__data__of__vector__[_tp_].__indx__ != __data__lengths__) {
							__data__lengths__ += 0x00001;
						}
					}
				}
				return __data__lengths__;
			}
		public:
			template<
				typename __data__template>
			__data__void__ update_number(__data__int__ __id__par__, __data__template __par__) {
				vector<__table__data__> __data__of__vector__temp__;
				__data__bool__ __its__rewrite__ = 0x0000;
				if(is_same<__data__template, __data__int__>::value) {
					for(__data__int__ _tp_ = 0x0000; _tp_ < ((__data__int__)__data__of__vector__.size()); _tp_++) {
						if(__data__of__vector__[_tp_].__indx__ == __id__par__) {
							if(!__its__rewrite__) {
								__table__data__ __data__register__;
								__data__register__.__indx__ = __id__par__;
								__data__register__.__type__ = _Type_Start;
								__data__register__.__data__ = (__data__double__)0x00001;
								__data__of__vector__temp__.push_back(__data__register__);

								__table__data__ __data__integer__;
								__data__integer__.__indx__ = __id__par__;
								__data__integer__.__type__ = _Type_Int;
								__data__integer__.__data__ = (__data__double__)__par__;
								__data__of__vector__temp__.push_back(__data__integer__);

								__table__data__ __data__push__;
								__data__push__.__indx__ = __id__par__;
								__data__push__.__type__ = _Type_Finish;
								__data__push__.__data__ = _Type_NULL;
								__data__of__vector__temp__.push_back(__data__push__);
								__its__rewrite__ = 0x00001;
							}
						} else {
							__data__of__vector__temp__.push_back(__data__of__vector__[_tp_]);
						}
					}
				} else if(is_same<__data__template, __data__float__>::value) {
					for(__data__int__ _tp_ = 0x0000; _tp_ < ((__data__int__)__data__of__vector__.size()); _tp_++) {
						if(__data__of__vector__[_tp_].__indx__ == __id__par__) {
							if(!__its__rewrite__) {
								__table__data__ __data__register__;
								__data__register__.__indx__ = __id__par__;
								__data__register__.__type__ = _Type_Start;
								__data__register__.__data__ = (__data__double__)0x00001;
								__data__of__vector__temp__.push_back(__data__register__);

								__table__data__ __data__integer__;
								__data__integer__.__indx__ = __id__par__;
								__data__integer__.__type__ = _Type_Float;
								__data__integer__.__data__ = (__data__double__)__par__;
								__data__of__vector__temp__.push_back(__data__integer__);

								__table__data__ __data__push__;
								__data__push__.__indx__ = __id__par__;
								__data__push__.__type__ = _Type_Finish;
								__data__push__.__data__ = _Type_NULL;
								__data__of__vector__temp__.push_back(__data__push__);
								__its__rewrite__ = 0x00001;
							}
						} else {
							__data__of__vector__temp__.push_back(__data__of__vector__[_tp_]);
						}
					}
				} else if(is_same<__data__template, __data__double__>::value) {
					for(__data__int__ _tp_ = 0x0000; _tp_ < ((__data__int__)__data__of__vector__.size()); _tp_++) {
						if(__data__of__vector__[_tp_].__indx__ == __id__par__) {
							if(!__its__rewrite__) {
								__table__data__ __data__register__;
								__data__register__.__indx__ = __id__par__;
								__data__register__.__type__ = _Type_Start;
								__data__register__.__data__ = (__data__double__)0x00001;
								__data__of__vector__temp__.push_back(__data__register__);

								__table__data__ __data__integer__;
								__data__integer__.__indx__ = __id__par__;
								__data__integer__.__type__ = _Type_Double;
								__data__integer__.__data__ = (__data__double__)__par__;
								__data__of__vector__temp__.push_back(__data__integer__);

								__table__data__ __data__push__;
								__data__push__.__indx__ = __id__par__;
								__data__push__.__type__ = _Type_Finish;
								__data__push__.__data__ = _Type_NULL;
								__data__of__vector__temp__.push_back(__data__push__);
								__its__rewrite__ = 0x00001;
							}
						} else {
							__data__of__vector__temp__.push_back(__data__of__vector__[_tp_]);
						}
					}
				}
				clear();
				for(__data__int__ _tp_ = 0x0000; _tp_ < ((__data__int__)__data__of__vector__temp__.size()); _tp_++) {
					__data__of__vector__.push_back(__data__of__vector__temp__[_tp_]);
				}
			}
			__data__void__ update_string(__data__int__ __id__par__, __data__string__ __par__) {
				vector<__table__data__> __data__of__vector__temp__;
				__data__bool__ __its__rewrite__ = 0x0000;
				for(__data__int__ _tp_ = 0x0000; _tp_ < ((__data__int__)__data__of__vector__.size()); _tp_++) {
					if(__data__of__vector__[_tp_].__indx__ == __id__par__) {
						if(!__its__rewrite__) {
							__table__data__ __data__register__;
								__data__register__.__indx__ = __id__par__;
								__data__register__.__type__ = _Type_Start;
								__data__register__.__data__ = (__data__double__)__par__.length();
								__data__of__vector__temp__.push_back(__data__register__);

								for(__data__int__ _tp_ = 0x0000; _tp_ < __par__.length(); _tp_++) {
									__table__data__ __data__char__;
									__data__char__.__indx__ = __id__par__;
									__data__char__.__type__ = _Type_Char;
									__data__char__.__data__ = 
										(__data__double__)__char__functional__->__id__of__char(__par__[_tp_]);
									__data__of__vector__temp__.push_back(__data__char__);
								}

								__table__data__ __data__push__;
								__data__push__.__indx__ = __id__par__;
								__data__push__.__type__ = _Type_Finish;
								__data__push__.__data__ = _Type_NULL;
								__data__of__vector__temp__.push_back(__data__push__);
								__its__rewrite__ = 0x00001;
						}
					} else {
						__data__of__vector__temp__.push_back(__data__of__vector__[_tp_]);
					}
				}

				clear();
				for(__data__int__ _tp_ = 0x0000; _tp_ < ((__data__int__)__data__of__vector__temp__.size()); _tp_++) {
					__data__of__vector__.push_back(__data__of__vector__temp__[_tp_]);
				}
			}
	};

	typedef class __binary__file__class binaryfile;
} // end of namespace std

#endif // end of header _BINARY_FILE