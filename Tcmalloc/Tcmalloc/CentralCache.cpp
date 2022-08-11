#include "CentralCache.h"

CentralCache CentralCache::inst;

Span* CentralCache::GetOneSpan(SpanList& list, size_t size)
{
	// ...
	return nullptr;
}
size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t batchNum, size_t size) {
	size_t index = SizeClass::Index(size);
	spanLists[index].mtx.lock();

	Span* span = GetOneSpan(spanLists[index], size);
	assert(span);
	assert(span->list);

	// 从span中获取batchNum个对象
	// 如果不够batchNum个，有多少拿多少
	start = span->list;
	end = start;
	size_t i = 0;
	size_t actualNum = 1;
	while (i < batchNum - 1 && NextObj(end) != nullptr)
	{
		end = NextObj(end);
		++i;
		++actualNum;
	}
	span->list = NextObj(end);
	NextObj(end) = nullptr;
	spanLists[index].mtx.unlock();

	return actualNum;
}