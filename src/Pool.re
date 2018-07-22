open SharedTypes;

[@bs.obj]
external _options :
    (
        ~acquireTimeout: int=?, ~waitForConnections: bool=?,
        ~connectionLimit: int=?, ~queueLimit: int=?, unit
    ) => poolOptions = "";

let options = (
    ~acquireTimeout=?,
    ~waitForConnections=?,
    ~connectionLimit=?,
    ~queueLimit=?,
    connectionOptions: connectionOptions
) : poolOptions => {
    let opts = _options(~acquireTimeout?, ~waitForConnections?, ~connectionLimit?, ~queueLimit?, ());
    Js.Array.concat(Js.Dict.entries(opts), Js.Dict.entries(connectionOptions))
        |> Js.Dict.fromArray;
};

type _t;
type t = Queryable.t(_t);

[@bs.module "mysql"] external make : poolOptions => t = "createPool";

[@bs.send] external _end : (t, Js.nullable(Js.Json.t) => unit) => unit = "end";

let end_ = (connection) =>
    Js.Promise.make((~resolve, ~reject) =>
        _end(connection, (err) =>
            switch (Js.toOption(err)) {
                | None => {
                    let u = ();
                    [@bs] resolve(u)
                }
                | Some(err) => [@bs] reject(JsonException(err))
            }
        )
    );
