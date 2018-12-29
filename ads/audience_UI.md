##1.How to add audience and KV in UI?

kv: advertising >>>> placement >>>> targeting >>>> custom targeting >>>> add new 
*This will be stored in table `kv_term`*
action: age=10,age=20, key001=value001

audience:network >>>> audience >>>> manage >>>> add new (can add new kv here, diff set)
action: audience002(set1:age=10,age=20 ; set2:key002=value002)
		audience001(set1:key001=value001)

```
DB change:
	key_value_term: id, 	key_value_string
					71629	key001=value001
					71630	key002=value002
					11192	age=10
					10066	age=20

	audience_item:  id,		network_id,		name,			creteria_id
					151096	501080			audience001		13460894	
					151097	501080			audience002		13460917

	targeting_criteria: audience targeting and ad targeting 
				id,		network_id,		name,		criteria_type_id,		relation,		root_criteria_id,		target_type	
				13460894	501080	root_for_...audience001		21			AND				13460894				AUDIENCE
				13460929	501080	Advanced_Criteria			21			OR				13460894				AUDIENCE
				13460930	501080	Set1						21			OR				13460894				AUDIENCE	

				13460917	501080	root_for_...audience002		21			AND				13460917				AUDIENCE
				13460923	501080	Advanced Criteria			21			OR				13460917				AUDIENCE	
				13460922	501080	Advanced Criteria			21			OR				13460917				AUDIENCE	
				13460924	501080	Set1						21			OR				13460917				AUDIENCE	
				13460925	501080	Set2						21			OR				13460917				AUDIENCE	

	
	targeting_criteria_assignment: relationship of above
				id		network_id  parent_criteria_id	child_criteria_id
				8099630		501080	13460894			13460929	
				8099631		501080	13460929			13460930	
				
				8099623		501080	13460917			13460922	
				8099624		501080	13460922			13460923	
				8099625		501080	13460923			13460924	
				8099626		501080	13460923			13460925	
	
	targeting_criteria_item_assignment: relationship between set and kv
				id			nw_id	targeting_cr_id 	cr_type		cr_value
				30852066	501080	13460924			12			10066(age=20)
				30852067	501080	13460924			12			11192(age=10)
				30852071	501080	13460930			12			71629(key001)	
				30852068	501080	13460925			12			71630(key002)	
					
```

*This will be stored in table `audience_item`*
##2.How to get audiene and plc targeting?
kv: advertising >>>> placement >>>> targeting >>>> custom targeting >>>> select audience iterm
audience: advertising >>>> plc >>>> targeting >>>> audience targeting >>>> select audience iterm

This will be stored into table 'targeting_criteria'

targeting type: cotent, ad, mrm_rule, audience
User setting:
plc-kv-targeting: age=10,age=20,key001=value001  : added to `targeting_criteria and targeting_criteria_item_assignment` 
plc-audience-targeting: audience002 

DB: ad_tree_node.criteria_id
