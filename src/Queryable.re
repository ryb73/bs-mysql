open SharedTypes;

type t('a);

[@bs.send]
external _query :
    (
        t('a), string,
        (Js.nullable(Js.Json.t), Js.nullable(Js.Json.t), Js.nullable(Js.Json.t)) => unit
    ) => unit = "query";

let query = (connection, query) =>
    Js.Promise.make((~resolve, ~reject) =>
        _query(connection, query, (err, result, fields) =>
            switch (Js.toOption(err)) {
                | Some(err) => [@bs] reject(JsonException(err))
                | None => {
                    let result = Js.Nullable.to_opt(result) |> Option.get;
                    [@bs] resolve((result, Js.Nullable.to_opt(fields)))
                }
            }
        )
    );
