open SharedTypes;

[@bs.obj]
external _options :
    (
        ~host: string=?, ~port: int=?, ~localAddress: string=?,
        ~socketPath: string=?, ~database: string=?, ~charset: string=?,
        ~timezone: string=?, ~connectTimeout: int=?, ~insecureAuth: Js.boolean=?,
        ~supportBigNumbers: Js.boolean=?, ~debug: Js.boolean=?, ~trace: Js.boolean=?,
        ~user: string=?, ~password: string=?, unit
    ) => connectionOptions = "";

let options = (
    ~host=?, ~port=?, ~localAddress=?, ~socketPath=?, ~database=?, ~charset=?,
    ~timezone=?, ~connectTimeout=?, ~insecureAuth=?, ~supportBigNumbers=?,
    ~debug=?, ~trace=?, ~user=?, ~password=?, ()
) => {
    _options(~host?, ~port?, ~localAddress?, ~socketPath?, ~database?, ~charset?,
        ~timezone?, ~connectTimeout?, ~user?, ~password?,
        ~insecureAuth=?Option.map(Js.Boolean.to_js_boolean, insecureAuth),
        ~supportBigNumbers=?Option.map(Js.Boolean.to_js_boolean, supportBigNumbers),
        ~debug=?Option.map(Js.Boolean.to_js_boolean, debug),
        ~trace=?Option.map(Js.Boolean.to_js_boolean, trace), ()
    );
};

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
