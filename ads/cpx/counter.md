## why we need counter?
1. control budget/volume.
2. global counters, each instance has a local counter
## How counter works?
## How should we use counter?
The are two types of counters:`ad counter && rule counter`
>ad counter
>0 : # of delivered budget   (price for flat rate ad)
>1 : # of delivered impression (for delivery control)
>2 : # of delivered click
>3 : # of delivered currency (for delivery control)

>rule counter
>0 : # of delivery
>1 : # of opportunity
> $$ rule counters will be reset periodically.

## Hints

## Inside code
1. Who provides the couter query service?

	 class Ads_Shared_Data_Service :  public Ads_Service_Base
	
2. How should get value of counter?

	int read_counter(Ads_GUID id, Ads_GUID ext_id, short type, int64_t& value, time_t& timestamp, bool local = true);

@value: return value.
@ext_id: RT
        EXT_ID_DAILY_PACING = 1,
        EXT_ID_DELAYED_ACK = 2,
        EXT_ID_TARGETED = 3,
        EXT_ID_CUSTOM_EVENT = 4,
        EXT_ID_UNMEASURED = 5	
@type: 
	
