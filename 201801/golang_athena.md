type:
```

type StartQueryExecutionInput struct {

    // A unique case-sensitive string used to ensure the request to create the query
    // is idempotent (executes only once). If another StartQueryExecution request
    // is received, the same response is returned and another query is not created.
    // If a parameter has changed, for example, the QueryString, an error is returned.
    //
    // This token is listed as not required because AWS SDKs (for example the AWS
    // SDK for Java) auto-generate the token for users. If you are not using the
    // AWS SDK or the AWS CLI, you must provide this token or the action will fail.
    ClientRequestToken *string `min:"32" type:"string" idempotencyToken:"true"`

    // The database within which the query executes.
    QueryExecutionContext *QueryExecutionContext `type:"structure"`

    // The SQL query statements to be executed.
    //
    // QueryString is a required field
    QueryString *string `min:"1" type:"string" required:"true"`

    // Specifies information about where and how to save the results of the query
    // execution.
    //
    // ResultConfiguration is a required field
    ResultConfiguration *ResultConfiguration `type:"structure" required:"true"`
    // contains filtered or unexported fields
}

type GetQueryExecutionInput struct {

    // The unique ID of the query execution.
    //
    // QueryExecutionId is a required field
    QueryExecutionId *string `type:"string" required:"true"`
    // contains filtered or unexported fields
}

type GetQueryResultsInput struct {

    // The maximum number of results (rows) to return in this request.
    MaxResults *int64 `type:"integer"`

    // The token that specifies where to start pagination if a previous request
    // was truncated.
    NextToken *string `type:"string"`

    // The unique ID of the query execution.
    //
    // QueryExecutionId is a required field
    QueryExecutionId *string `type:"string" required:"true"`
    // contains filtered or unexported fields
}


====output
type StartQueryExecutionOutput struct {

    // The unique ID of the query that ran as a result of this request.
    QueryExecutionId *string `type:"string"`
    // contains filtered or unexported fields
}

type GetQueryExecutionOutput struct {

    // Information about the query execution.
    QueryExecution *QueryExecution `type:"structure"`
    // contains filtered or unexported fields
}

type QueryExecution struct {

    // The SQL query statements which the query execution ran.
    Query *string `min:"1" type:"string"`

    // The database in which the query execution occurred.
    QueryExecutionContext *QueryExecutionContext `type:"structure"`

    // The unique identifier for each query execution.
    QueryExecutionId *string `type:"string"`

    // The location in Amazon S3 where query results were stored and the encryption
    // option, if any, used for query results.
    ResultConfiguration *ResultConfiguration `type:"structure"`

    // The amount of data scanned during the query execution and the amount of time
    // that it took to execute.
    Statistics *QueryExecutionStatistics `type:"structure"`

    // The completion date, current state, submission time, and state change reason
    // (if applicable) for the query execution.
    Status *QueryExecutionStatus `type:"structure"`
    // contains filtered or unexported fields
}

type QueryExecutionStatus struct {

    // The date and time that the query completed.
    CompletionDateTime *time.Time `type:"timestamp" timestampFormat:"unix"`

    // The state of query execution. SUBMITTED indicates that the query is queued
    // for execution. RUNNING indicates that the query is scanning data and returning
    // results. SUCCEEDED indicates that the query completed without error. FAILED
    // indicates that the query experienced an error and did not complete processing.
    // CANCELLED indicates that user input interrupted query execution.
    State *string `type:"string" enum:"QueryExecutionState"`

    // Further detail about the status of the query.
    StateChangeReason *string `type:"string"`

    // The date and time that the query was submitted.
    SubmissionDateTime *time.Time `type:"timestamp" timestampFormat:"unix"`
    // contains filtered or unexported fields
}

==part3
下面是查询的最终结果.
type GetQueryResultsOutput struct {

    // A token to be used by the next request if this request is truncated.
    NextToken *string `type:"string"`

    // The results of the query execution.
    ResultSet *ResultSet `type:"structure"`
    // contains filtered or unexported fields
}


```

operations:

```
==part1
func (c *Athena) StartQueryExecution(input *StartQueryExecutionInput) (*StartQueryExecutionOutput, error)
通过这个函数，我们由StartQueryExecutionInput得到了StartQueryExecutionOutput， 实际上是由一个queryString得到了一个queryId.

func (c *Athena) StartQueryExecutionRequest(input *StartQueryExecutionInput) (req *request.Request, output *StartQueryExecutionOutput)
上面两个函数类似, 这个函数目前我们可以认为是一个同步操作。

==part2
func (c *Athena) GetQueryExecution(input *GetQueryExecutionInput) (*GetQueryExecutionOutput, error)
需要明白的是，这个函数是异步返回的，也就是说，即使queryId对应的Input还没有结束，我们也能得到查询状态。理论上说，这是一个针对查询状态的查询，而不是一个针对查询结果的调用.

func (c *Athena) GetQueryExecutionRequest(input *GetQueryExecutionInput) (req *request.Request, output *GetQueryExecutionOutput)

==part3
func (c *Athena) GetQueryResults(input *GetQueryResultsInput) (*GetQueryResultsOutput, error)

func (c *Athena) GetQueryResultsRequest(input *GetQueryResultsInput) (req *request.Request, output *GetQueryResultsOutput)

```
