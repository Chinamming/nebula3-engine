#pragma once
//------------------------------------------------------------------------------
/**
    @class Debug::DebugPageHandler
    
    Http request handler for the Debug subsystem.
    
    (C) 2008 Radon Labs GmbH
*/
#include "http/httprequesthandler.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace Debug
{
class DebugPageHandler : public Http::HttpRequestHandler
{
    __DeclareClass(DebugPageHandler);
public:
    /// constructor
    DebugPageHandler();
    /// handle a http request
    virtual void HandleRequest(const Ptr<Http::HttpRequest>& request);

protected:
    /// compute the min, max and average time from an array of times
    void ComputeMinMaxAvgTimes(const Util::Array<Timing::Time>& times, Timing::Time& outMin, Timing::Time& outMax, Timing::Time& outAvg) const;
    /// compute min, max and average values from an array of counter samples
    void ComputeMinMaxAvgCounts(const Util::Array<int>& counterValues, int& outMin, int& outMax, int& outAvg) const;

private:
    /// handle HTTP request for a debug timer
    void HandleTimerRequest(const Util::String& timerName, const Ptr<Http::HttpRequest>& request);
    /// handle HTTP request for a debug counter
    void HandleCounterRequest(const Util::String& counterName, const Ptr<Http::HttpRequest>& request);
    /// handle HTTP request to render a timer chart
    void HandleTimerChartRequest(const Util::String& timerName, const Ptr<Http::HttpRequest>& request);
    /// handle HTTP request to render a counter char
    void HandleCounterChartRequest(const Util::String& counterName, const Ptr<Http::HttpRequest>& request);
    /// handle HTTP request to sort table
    void HandleTableSortRequest(const Util::String& columnName, const Ptr<Http::HttpRequest>& request);

    Util::String sortByColumn;
};

} // namespace Debug
//------------------------------------------------------------------------------

