open SharedTypes;

[@bs.obj]
external options :
    (
        ~host: string=?, ~port: int=?, ~localAddress: string=?,
        ~socketPath: string=?, ~database: string=?, ~charset: string=?,
        ~timezone: string=?, ~connectTimeout: int=?, ~insecureAuth: bool=?,
        ~supportBigNumbers: bool=?, ~debug: bool=?, ~trace: bool=?,
        ~user: string=?, ~password: string=?, unit
    ) => connectionOptions = "";

type _t;
type t = Queryable.t(_t);

[@bs.module "mysql"] external make : connectionOptions => t = "createConnection";

[@bs.send] external _end : (t, Js.nullable(Js.Json.t) => unit) => unit = "end";

let end_ = (connection) =>
    Js.Promise.make((~resolve, ~reject) =>
        _end(connection, (err) =>
            switch (Js.toOption(err)) {
                | None => let u = (); [@bs] resolve(u)
                | Some(err) => [@bs] reject(JsonException(err))
            }
        )
    );

[@bs.send] external destroy : t => unit = "";
