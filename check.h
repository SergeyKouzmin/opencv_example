#ifndef CHECK_H_
#define CHECK_H_

#include <string>
#include <vector>
#include <limits>
#include <time.h>

#ifdef _MSC_VER
#include <windows.h>
#undef min
#undef max
#pragma warning( push )
#pragma warning( disable : 4996 )
#endif

/**
 * Выкидывает исключение типа std::string с указанием аргументов функции.
 *
 * @param[in] fileName имя файла, в котором произошла ошибка.
 * @param[in] lineNumber десятичная запись номера строки, в которой произошла ошибка.
 * @param[in] failedExpression изображение выражения, оказавшегося ложным.
 * @throws std::string сообщение о месте ошибки и выражении, не прошедшем проверку.
 *        Исключение порождается всегда.
 */
inline void throwString(std::string fileName, std::string lineNumber, std::string failedExpression) {
#ifdef _MSC_VER
	if (IsDebuggerPresent()) {
		DebugBreak();
	}
#endif
	throw "Check failed: " + failedExpression + ", file " + fileName + ", line " + lineNumber;
}

/**
 * Выкидывает исключение типа std::string с указанием аргументов функции.
 *
 * @param[in] fileName имя файла, в котором произошла ошибка.
 * @param[in] lineNumber десятичная запись номера строки, в которой произошла ошибка.
 * @param[in] failedExpression изображение выражения, оказавшегося ложным.
 * @param[in] comment дополнительная информация, которая может уточнить характер проблемы.
 * @throws std::string сообщение о месте ошибки, выражении, не прошедшем проверку, и дополнительной информацией.
 *        Исключение порождается всегда.
 */
inline void throwString(std::string fileName, std::string lineNumber, std::string failedExpression, std::string comment) {
#ifdef _MSC_VER
	if (IsDebuggerPresent()) {
		DebugBreak();
	}
#endif
	throw "Check failed: " + failedExpression + " (" + comment + "), file " + fileName + ", line " + lineNumber;
}

/**
 * Выкидывает исключение типа std::string с указанием аргументов функции.
 *
 * @param[in] fileName имя файла, в котором произошла ошибка.
 * @param[in] lineNumber десятичная запись номера строки, в которой произошла ошибка.
 * @param[in] failedExpression изображение выражения, оказавшегося ложным.
 * @param[in] comment дополнительная информация, которая может уточнить характер проблемы.
 * @param[in] comment2 дополнительная информация, которая может уточнить характер проблемы.
 * @throws std::string сообщение о месте ошибки, выражении, не прошедшем проверку, и дополнительной информацией.
 *        Исключение порождается всегда.
 */
inline void throwString(std::string fileName, std::string lineNumber, std::string failedExpression, std::string comment, char comment2) {
	throwString(fileName, lineNumber, failedExpression, comment + " " + comment2);
}

/**
 * Выкидывает исключение типа std::string с указанием аргументов функции.
 *
 * @param[in] fileName имя файла, в котором произошла ошибка.
 * @param[in] lineNumber десятичная запись номера строки, в которой произошла ошибка.
 * @param[in] failedExpression изображение выражения, оказавшегося ложным.
 * @param[in] comment дополнительная информация, которая может уточнить характер проблемы.
 * @param[in] comment2 дополнительная информация, которая может уточнить характер проблемы.
 * @throws std::string сообщение о месте ошибки, выражении, не прошедшем проверку, и дополнительной информацией.
 *        Исключение порождается всегда.
 */
inline void throwString(std::string fileName, std::string lineNumber, std::string failedExpression, std::string comment, std::string comment2) {
	throwString(fileName, lineNumber, failedExpression, comment + " " + comment2);
}

/**
 * Выкидывает исключение типа std::string с указанием аргументов функции.
 *
 * @param[in] fileName имя файла, в котором произошла ошибка.
 * @param[in] lineNumber десятичная запись номера строки, в которой произошла ошибка.
 * @param[in] failedExpression изображение выражения, оказавшегося ложным.
 * @param[in] comment дополнительная информация, которая может уточнить характер проблемы.
 * @param[in] comment2 дополнительная информация, которая может уточнить характер проблемы.
 * @param[in] comment3 дополнительная информация, которая может уточнить характер проблемы.
 * @throws std::string сообщение о месте ошибки, выражении, не прошедшем проверку, и дополнительной информацией.
 *        Исключение порождается всегда.
 */
inline void throwString(std::string fileName, std::string lineNumber, std::string failedExpression, std::string comment, std::string comment2, std::string comment3) {
	throwString(fileName, lineNumber, failedExpression, comment + " " + comment2 + " " + comment3);
}

#ifdef QUOTE
#error QUOTE macro defined not only in check.h
#endif

#ifdef QUOTE_VALUE
#error QUOTE_VALUE macro defined not only in check.h
#endif

#ifdef check
#error check macro defined not only in check.h
#endif

/**
 * Преобразует аргумент в строковый литерал с его изображением.
 */
#define QUOTE(x) #x

/**
 * Преобразует аргумент в строковый литерал с его изображением,
 * при этом макроопределения вроде __LINE__ заменяются их значениями.
 * QUOTE(__LINE__) - это "__LINE__", QUOTE_VALUE(__LINE__) - это "100".
 */
#define QUOTE_VALUE(x) QUOTE(x)

/**
 * Проверяет, что выражение @a expression истинно. Если выражение истинно,
 * ничего не делает. Если ложно &mdash; выкидывает исключение типа std::string,
 * содержащее имя файла и строки, где записан вызов этого макроса,
 * выражение, не прошедшее проверку, а также значения дополнительных
 * параметров, если они заданы.
 *
 * Макрос очень похож на assert, однако разница в том, что assert по определению
 * вещь отключаемая (обычно в релиз-версии), а отключение check невозможно.
 * Поэтому в критических по производительности местах нужно использовать assert,
 * во всех остальных check. Второе различие assert и check в том, что срабатывание
 * assert вызывает завершение программы, а срабатывание check всего лишь порождает
 * исключение, так что его можно использовать для не только для проверки логических
 * ошибок, но и аргументов функций, и кодов возврата.
 */
#define check(expression, ...) \
{ \
	if (!(expression)) { \
		throwString(__FILE__, QUOTE_VALUE(__LINE__), #expression, ##__VA_ARGS__); \
	} \
}

#ifdef _MSC_VER
#define CURRENT_FUNCTION __FUNCSIG__
#elif __GNUG__
#define CURRENT_FUNCTION __PRETTY_FUNCTION__
#else
#define CURRENT_FUNCTION __FILE__ "(" QUOTE_VALUE(__LINE__) ")"
#endif

#define BEGIN_FUNCTION \
	try

#define END_FUNCTION \
	catch (const std::string &message) { \
		throw CURRENT_FUNCTION + (" " + message); \
	}

class ExceptionReporter {
public:

	/**
	 * Выводит в регистрирующий поток сообщение о пойманном исключении @a message.
	 *
	 * @param[in] message сообщение о пойманном исключении.
	 */
	virtual void report(std::string message) = 0;
};

/**
 * Класс, сообщающий об исключении.
 */
class ExceptionReporterList {
private:

	static void addDefaultExceptionReporters();

	/**
	 * Возвращает ссылку на список объектов, регистрирующих пойманные исключения.
	 * Ссылка на статическую переменную внутри функции &mdash; полный аналог
	 * статического поля, однако не требует объявлений в cpp-файле.
	 *
	 * @return ссылку на список объектов, регистрирующих пойманные исключения.
	 */
	static std::vector<ExceptionReporter*> & list() {
		static std::vector<ExceptionReporter*> list;
		static bool firstCall = true;
		if (firstCall) {
			firstCall = false;
			addDefaultExceptionReporters();
		}
		return list;
	}

public:

	static void addExceptionReporter(ExceptionReporter *exceptionReporter) {
		list().push_back(exceptionReporter);
	}

	/**
	 * Сообщает всем объектам, регистрирующим пойманные исключения, об исключении
	 * с описанием @a message, пойманном в строке @a lineNumber файла @a fileName.
	 *
	 * @param[in] fileName имя файла, в котором было поймано исключение.
	 * @param[in] lineNumber десятичная запись номера строки, в которой было поймано исключение.
	 * @param[in] message описание исключения.
	 */
	static void reportException(std::string fileName, std::string lineNumber, std::string message) {
		std::string strTime;
		{
			time_t timer = ::time(NULL);
			if (timer != -1) {
				char buffer[128];
				size_t result = strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S ", ::localtime(&timer));
				if (result > 0) {
					strTime = buffer;
				}
			}
		}
		std::string completeMessage = strTime + "Exception caught in file " + fileName + " line " + lineNumber + ": " + message;
		for (std::vector<ExceptionReporter*>::size_type i = 0; i < list().size(); i++) {
			list()[i]->report(completeMessage);
		}
	}
};

/**
 * Обёртывает аргументы в блок try-catch, не выпускающий исключений,
 * а передающий их ExceptionReporter.
 *
 * Типичное использование &mdash; в местах, которые исключение ни при каких
 * обстоятельствах не должно покинуть, например, в деструкторах &mdash;
 * в случае, если мы всё-таки хотим знать о произошедших ошибках.
 * <pre>
 * ~Object() {
 *     BEGIN_DESTRUCTOR {
 *         func1();
 *         func2();
 *     } END_DESTRUCTOR
 * }
 * </pre>
 * Фигурные скобки вне макроса делают правильным автоотступ в Visual Studio 2008.
 */
#define BEGIN_DESTRUCTOR \
	try { \
		try

#define END_DESTRUCTOR \
		catch (std::string message) { \
			ExceptionReporterList::reportException(__FILE__, QUOTE_VALUE(__LINE__), message); \
		} catch (...) { \
			ExceptionReporterList::reportException(__FILE__, QUOTE_VALUE(__LINE__), "unknown exception"); \
		} \
	} catch (...) {}

/**
 * Возвращает значение аргумента @a src, изменяя его тип с SourceType на DestinationType.
 * В случае, если значение @a src не принадлежит типу DestinationType, выкидывается
 * исключение типа std::string.
 *
 * @param[in] src значение, тип которого нужно изменить с SourceType на DestinationType.
 * @return значение @a src, но уже типа DestinationType.
 * @throws std::string если значение @a src не принадлежит типу DestinationType.
 */
template<typename DestinationType, typename SourceType>
DestinationType checked_cast(SourceType src) {
	DestinationType dst = static_cast<DestinationType>(src);
	check(static_cast<SourceType>(dst) == src, "value changed by type cast");
	if (
		(std::numeric_limits<SourceType>::min() == 0 && std::numeric_limits<DestinationType>::min() != 0) ||
		(std::numeric_limits<SourceType>::min() != 0 && std::numeric_limits<DestinationType>::min() == 0)
	) {
		check((src > 0 && dst > 0) || (src <= 0 && dst <= 0), "value changed by type cast");
	}
	return dst;
}

/**
 * Возвращает результат умножения @a first на @a second. Выкидывает исключение
 * типа std::string в случае переполнения.
 *
 * @param[in] first первый множитель.
 * @param[in] second второй множитель.
 * @return результат умножения.
 * @throws std::string если значение произведения не принадлежит типу аргументов.
 */
template<typename T>
T checked_mul(T first, T second) {
	T product = first * second;
	check(first == 0 || product / first == second, "an int multiplication overflow");
	return product;
}

template<typename SizeType, typename ValueType>
ValueType multiply_sizeof(ValueType value) {
	return checked_mul(value, checked_cast<ValueType>(sizeof(SizeType)));
}

/**
 * Возвращает результат сложения @a first и @a second. Выкидывает исключение
 * типа std::string в случае переполнения.
 *
 * @param[in] first первое слагаемое.
 * @param[in] second второе слагаемое.
 * @return результат сложения.
 * @throws std::string если значение суммы не принадлежит типу аргументов.
 */
template<typename T>
T checked_add(T first, T second) {
	T sum = first + second;
	if (first <= 0 && second <= 0) {
		check(sum <= 0, "an int addition overflow");
	} else if (first > 0 && second > 0) {
		check(sum > 0, "an int addition overflow");
	}
	return sum;
}

/**
 * Конвертирует строку в число типа int. Выкидывает исключение
 * типа std::string в случае переполнения либо некорректности формата числа.
 *
 * @param[in] str строка, которую нужно преобразовать в число типа int.
 * @return результат преобразования в число типа int.
 * @throws std::string если строка имеет неправильный формат, либо имеет место переполнение.
 */

#include <stdlib.h>
#include <errno.h>


inline int checked_atoi(const std::string &str) {
	char * ptrEnd;
#ifdef _MSC_VER
	_set_errno(0);
#else
	errno = 0;
#endif
	int result = checked_cast<int>(strtol(str.c_str(), &ptrEnd , 10));
	check(('0' <= str[0] && str[0] <= '9') || str[0] == '-', std::string(str + " is not a number"));
	check(errno == 0, std::string(str + " : int overflow"));
	check(*ptrEnd == '\0', std::string(str + " is not a number"));
	return result;
}

/**
 * Конвертирует строку в число типа double. Выкидывает исключение
 * типа std::string в случае переполнения либо некорректности формата числа.
 *
 * @param[in] str строка, которую нужно преобразовать в число типа double.
 * @return результат преобразования в число типа double.
 * @throws std::string если строка имеет неправильный формат, либо имеет место переполнение.
 */
inline double checked_atof(const std::string &str) {
	char * ptrEnd;
#ifdef _MSC_VER
	_set_errno(0);
#else
	errno = 0;
#endif
	double result = strtod(str.c_str(), &ptrEnd);
	check(('0' <= str[0] && str[0] <= '9') || str[0] == '-' || str[0] == '.', std::string(str + " is not a number"));
	check(errno == 0, std::string(str + " : double overflow"));
	check(*ptrEnd == '\0', std::string(str + " is not a number"));
	return result;
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // CHECK_H_
