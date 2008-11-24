//
// MySQLException.cpp
//
// $Id: //poco/1.4/Data/MySQL/src/Extractor.cpp#1 $
//
// Library: Data
// Package: MySQL
// Module:  Extractor
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/Data/MySQL/Extractor.h"

#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"

namespace Poco {
namespace Data {
namespace MySQL {


Extractor::Extractor(StatementExecutor& st, ResultMetadata& md): _stmt(st), _metadata(md)
{
}


Extractor::~Extractor()
{
}


bool Extractor::extract(std::size_t pos, Poco::Int8& val)
{
	return realExtractFixed(pos, MYSQL_TYPE_TINY, &val);
}


bool Extractor::extract(std::size_t pos, Poco::UInt8& val)
{
	return realExtractFixed(pos, MYSQL_TYPE_TINY, &val);
}


bool Extractor::extract(std::size_t pos, Poco::Int16& val)
{
	return realExtractFixed(pos, MYSQL_TYPE_SHORT, &val);
}


bool Extractor::extract(std::size_t pos, Poco::UInt16& val)
{
	return realExtractFixed(pos, MYSQL_TYPE_SHORT, &val);
}


bool Extractor::extract(std::size_t pos, Poco::Int32& val)
{
	return realExtractFixed(pos, MYSQL_TYPE_LONG, &val);
}


bool Extractor::extract(std::size_t pos, Poco::UInt32& val)
{
	return realExtractFixed(pos, MYSQL_TYPE_LONG, &val);
}


bool Extractor::extract(std::size_t pos, Poco::Int64& val)
{
	return realExtractFixed(pos, MYSQL_TYPE_LONGLONG, &val);
}


bool Extractor::extract(std::size_t pos, Poco::UInt64& val)
{
	return realExtractFixed(pos, MYSQL_TYPE_LONGLONG, &val);
}


#ifndef POCO_LONG_IS_64_BIT
bool Extractor::extract(std::size_t pos, long& val)
		/// Extracts a long. Returns false if null was received.
{
	return realExtractFixed(pos, MYSQL_TYPE_LONGLONG, &val);
}
#endif


bool Extractor::extract(std::size_t pos, bool& val)
{
	return realExtractFixed(pos, MYSQL_TYPE_TINY, &val);
}


bool Extractor::extract(std::size_t pos, float& val)
{
	return realExtractFixed(pos, MYSQL_TYPE_FLOAT, &val);
}

	
bool Extractor::extract(std::size_t pos, double& val)
{
	return realExtractFixed(pos, MYSQL_TYPE_DOUBLE, &val);
}

	
bool Extractor::extract(std::size_t pos, char& val)
{
	return realExtractFixed(pos, MYSQL_TYPE_TINY, &val);
}

	
bool Extractor::extract(std::size_t pos, std::string& val)
{
	if (_metadata.columnsReturned() <= pos)
		throw MySQLException("Extractor: attempt to extract more paremeters, than query result contain");
	
	if (_metadata.isNull(static_cast<Poco::UInt32>(pos)))
	return false;
	
	if (_metadata.metaColumn(static_cast<Poco::UInt32>(pos)).type() != Poco::Data::MetaColumn::FDT_STRING)
		throw MySQLException("Extractor: not a string");
	
	val.assign(reinterpret_cast<const char*>(_metadata.rawData(pos)), _metadata.length(pos));
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Data::BLOB& val)
{
	if (_metadata.columnsReturned() <= pos)
		throw MySQLException("Extractor: attempt to extract more paremeters, than query result contain");
	
	if (_metadata.isNull(static_cast<Poco::UInt32>(pos)))
	return false;
	
	if (_metadata.metaColumn(static_cast<Poco::UInt32>(pos)).type() != Poco::Data::MetaColumn::FDT_BLOB)
		throw MySQLException("Extractor: not a blob");
	
	val.assignRaw(_metadata.rawData(pos), _metadata.length(pos));
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Data::CLOB& val)
{
	if (_metadata.columnsReturned() <= pos)
		throw MySQLException("Extractor: attempt to extract more paremeters, than query result contain");
	
	if (_metadata.isNull(static_cast<Poco::UInt32>(pos)))
	return false;
	
	if (_metadata.metaColumn(static_cast<Poco::UInt32>(pos)).type() != Poco::Data::MetaColumn::FDT_BLOB)
		throw MySQLException("Extractor: not a blob");
	
	val.assignRaw(reinterpret_cast<const char*>(_metadata.rawData(pos)), _metadata.length(pos));
	return true;
}


bool Extractor::extract(std::size_t pos, DateTime& val)
{
	MYSQL_TIME mt = {0};

	if (!realExtractFixed(pos, MYSQL_TYPE_DATETIME, &mt))
		return false;

	val.assign(mt.year, mt.month, mt.day, mt.hour, mt.minute, mt.second, mt.second_part, 0);
	return true;
}


bool Extractor::extract(std::size_t pos, Date& val)
{
	MYSQL_TIME mt = {0};

	if (!realExtractFixed(pos, MYSQL_TYPE_DATE, &mt))
		return false;

	val.assign(mt.year, mt.month, mt.day);
	return true;
}


bool Extractor::extract(std::size_t pos, Time& val)
{
	MYSQL_TIME mt = {0};

	if (!realExtractFixed(pos, MYSQL_TYPE_TIME, &mt))
		return false;

	val.assign(mt.hour, mt.minute, mt.second);
	return true;
}


bool Extractor::extract(std::size_t pos, Any& val)
{
	return false;
}


bool Extractor::extract(std::size_t pos, Dynamic::Var& val)
{
	return false;
}


bool Extractor::isNull(std::size_t col, std::size_t row)
{
	poco_assert(row == POCO_DATA_INVALID_ROW);

	if (_metadata.columnsReturned() <= col)
		throw MySQLException("Extractor: attempt to extract more paremeters, than query result contain");

	if (_metadata.isNull(static_cast<Poco::UInt32>(col)))
		return true;

	return false;
}

void Extractor::reset()
{
	AbstractExtractor::reset();
}


bool Extractor::realExtractFixed(std::size_t pos, enum_field_types type, void* buffer, size_t length)
{
	MYSQL_BIND bind = {0};
	my_bool isNull = 0;

	bind.is_null	   = &isNull;
	bind.buffer_type   = type;
	bind.buffer		= buffer;
	bind.buffer_length = static_cast<unsigned long>(length);
	
	if (!_stmt.fetchColumn(pos, &bind))
		return false;

	return isNull == 0;
}


//////////////
// Not implemented
//////////////


bool Extractor::extract(std::size_t , std::vector<Poco::Int8>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Poco::Int8>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Poco::Int8>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}
	
	
bool Extractor::extract(std::size_t , std::vector<Poco::UInt8>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Poco::UInt8>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Poco::UInt8>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Poco::Int16>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Poco::Int16>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Poco::Int16>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Poco::UInt16>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Poco::UInt16>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Poco::UInt16>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Poco::Int32>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Poco::Int32>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Poco::Int32>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Poco::UInt32>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Poco::UInt32>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Poco::UInt32>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Poco::Int64>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Poco::Int64>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Poco::Int64>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Poco::UInt64>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Poco::UInt64>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Poco::UInt64>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


#ifndef POCO_LONG_IS_64_BIT
bool Extractor::extract(std::size_t , std::vector<long>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<long>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<long>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}
#endif


bool Extractor::extract(std::size_t , std::vector<bool>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<bool>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<bool>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<float>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<float>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<float>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<double>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<double>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<double>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<char>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<char>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<char>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<std::string>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<std::string>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<std::string>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<BLOB>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<BLOB>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<BLOB>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<CLOB>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<CLOB>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<CLOB>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<DateTime>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<DateTime>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<DateTime>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Date>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Date>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Date>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Time>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Time>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Time>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Any>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Any>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Any>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Dynamic::Var>& )
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Dynamic::Var>& )
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Dynamic::Var>& )
{
	throw NotImplementedException("std::list extractor must be implemented.");
}



} } } // namespace Poco::Data::MySQL
