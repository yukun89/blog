## How to generate struct for a table in DB?
### 1. add a new table to MRM.py

file:	utils/pusher/MRM.py
content: `in section TABLE` 
```
CREATE TEMPORARY TABLE `abstract_event` (
    `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'This table describes network abstract event',
    `network_id` bigint(20) COMMENT 'FK to network.id',
    `name` varchar(255) NOT NULL COMMENT 'Abstract event name',
    `status` enum('ACTIVE', 'INACTIVE') DEFAULT 'ACTIVE',
    `created_at` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
    `updated_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY (`id`)
);
```
----
### 2. generate the struct in 

`python ../utils/pusher/generate_pusher.py --type=MRM --dsn='DSN=fwmrm_oltp_;UID=ads;PWD=ads' > Ads_Pusher_DB_View.h`


----
### 3. Implement code in Ads_Pusher_DB_View.cpp 
The most important function is: Ads_Pusher::load_repository(Ads_Repository *repo, Ads_Depository *depo, int flags, const Ads_Repository *base_repo) 

1. create db connection: `Ads_Pusher::DB_Connection db_oltp`
2. create temp table and store data: `Ads_Pusher_Base::prepare_temporary_table(db_oltp, FEEDBACK_FILE_PATTERN_CPX_PLACEMENT, meta_dir + "/cpx_placement", table, create_table, "")`
	2.1 get most recent file: `Ads_Pusher_Service::most_recent_file(dir, pattern, path, dummy_index, time, version)`
	2.2 create temp table:`conn.execute(create_table.c_str()`
	2.3 load data:`conn.execute(query.c_str())`
3. load db view: `db_oltp.load<DB_View_cpx_placement_feedback>(repo, depo)`
				`template <typename View> int load(Ads_Repository *repo, Ads_Depository * depo, bool failure_as_error = true,const Ads_String& backfill_query = "")`
				`Ads_Pusher_Base::DB_Connection::load(DB_View& view, Ads_Repository *repo, Ads_Depository *depo, bool failure_as_error)`
				`view.handle_record()`

####3.1 create table if needed

####3.2 load<db_view>

load db view: `db_oltp.load<DB_View_cpx_placement_feedback>(repo, depo)`

####3.3 handle_record
example:
```
int
DB_View_cpx_placement_feedback::handle_record(Ads_Repository *repo, Ads_Depository *depo)
{
    Ads_Advertisement_Map &advertisements = (this->delta_repo_ ? this->delta_repo_->advertisements_ : repo->advertisements_);
    Ads_Advertisement_Map::iterator it = advertisements.find(ads::entity::make_id(ADS_ENTITY_TYPE_ADVERTISEMENT, do_.placement_id));
    if(it != advertisements.end())
    {
        Ads_Advertisement *placement= it->second;
        if(do_.compositional_measured_ad_views > 0 && do_.compositional_recorded_events >= 0)
                placement->compositional_ratio_ = double(do_.compositional_recorded_events)/do_.compositional_measured_ad_views;//TODO
        else{
                placement->compositional_ratio_ = 0.5;
        }
        placement->delivered_imps_ = do_.mrm_ad_views;
        placement->recorded_abstract_events_ = do_.recorded_abstract_events;
        placement->unmeasured_imps_ = do_.unmeasured_ad_views;
        placement->expense_ = ADS_TO_MAGNIFIED_CURRENCY(strtod(do_.mrm_currency,0));
        placement->recorded_abstract_event_currency_ = ADS_TO_MAGNIFIED_CURRENCY(strtod(do_.recorded_abstract_event_currency,0));
        placement->unmeasured_expense_ = ADS_TO_MAGNIFIED_CURRENCY(strtod(do_.unmeasured_currency,0));
        return 0;
    }
	return 0;
}
```
For more [info](http://wiki.dev.fwmrm.net/display/viqa/DB+Pusher+Introduction)

