## audience ingest

Q: How to get data from userDB?

http://wiki.dev.fwmrm.net/display/~xlzhu/AdServing+Learn+Audience
http://wiki.dev.fwmrm.net/display/MUGPublicDrafts/How+to+Ingest+Audience+User+Records
http://wiki.dev.fwmrm.net/display/audience/4.+How+to+enable+Audience+Management

### 1. user_id resource?

* FW UserID (in cookie)
* Visitor Custom ID
* Mobile Device ID  

### 2. User Mapping

 User Mapping scenario
* Data Provider is responsible  for User Mapping
>exchange url contains fw user id and redirect to DP

* FW is responsible for User Mapping
>exchange url contains a redirect url which will be redirect to FW with DP user id

3. Use Scenarios

see the link above.

## audience data

### 1.how to ge audience data from userdb? what KV in UserDB like?

original(http://wiki.dev.fwmrm.net/display/wq/AdServing+Test+FDB-13811+Audience+Database+Customer+Facing+Query+Tool+for+Diagnostics).

dispatch: http://nycadsdebug.fwmrm.net:1080/dispatch?command=server::query_user&user_id=ads40_6011266893548677557&nw=48804&dpid=48804

by:`[user_id, customer_id], network_id, data_provider_id`

kv in audience like:

```

_fwu:174053:a1
_fwu:174053:age=25-30
_fwu:174053:b1
_fwu:174053:c1
_fwu:174053:k1
_fwu:174053:k10
_fwu:174053:k100
_fwu:174053:k11
_fwu:174053:k12
_fwu:174053:k13
_fwu:174053:k14
_fwu:174053:k15
```

format: `_fwu:data_provider_id:kv_string`. That is different kvs in UserDB can not overlap.

**Q:How to use thi data? Is there difference for `_fwu:111:age=20` and `_fwu:222:aget=20` ?**

