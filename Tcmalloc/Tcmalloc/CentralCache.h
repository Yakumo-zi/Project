#pragma once
#include "Common.h"


class CentralCache {
public:
	static CentralCache* GetInstance()
	{
		return &inst;
	}
	size_t FetchRangeObj(void*& start, void*& end, size_t batchNum, size_t size);
	Span*  GetOneSpan(SpanList& list, size_t size);
private:
	CentralCache(){};
	CentralCache(const CentralCache&) = delete;
private:
	SpanList spanLists[NFREE_LISTS];
	static CentralCache inst;
};