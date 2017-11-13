open SharedTypes;

external _options : acquireTimeout::int? => waitForConnections::Js.boolean? =>
    connectionLimit::int? => queueLimit::int? => unit => poolOptions = "" [@@bs.obj];
let options ::acquireTimeout=? ::waitForConnections=? ::connectionLimit=?
    ::queueLimit=? (connectionOptions : connectionOptions) : poolOptions =>
{
    let opts = _options ::?acquireTimeout ::?waitForConnections ::?connectionLimit
        ::?queueLimit ();

    Js.Array.concat (Js.Dict.entries opts) (Js.Dict.entries connectionOptions)
        |> Js.Dict.fromArray;
};

type _t;
type t = Queryable.t _t;
external make : poolOptions => t = "createPool" [@@bs.module "mysql"];

external _end : t => (Js.nullable Js.Json.t => unit) => unit = "end" [@@bs.send];
let end_ connection => {
    Js.Promise.make (fun ::resolve ::reject => {
        _end connection (fun err => {
            switch (Js.toOption err) {
                | None => let u = (); resolve u [@bs]
                | Some err => reject (JsonException err) [@bs]
            };
        });
    });
};