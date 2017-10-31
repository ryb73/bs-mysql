open Js.Promise;
open Option.Infix;
open BatPervasives;

type config = {
    host: string,
    user: string,
    password: string,
    database: string
};
let config = switch (Config.get "test" |> config__from_json) {
    | Error _ => failwith "Error loading config"
    | Ok c => c
};

let opts = Mysql.connectionOptions host::config.host user::config.user
    password::config.password database::config.database ();
let conn = Mysql.createConnection opts;

let doInsert () => Mysql.query conn "INSERT INTO test (str, i, b, f, d, dt)
                    VALUES ('hey', 48, false, 4.23, 3.24, '2017-10-31 14:14:00');";
let doSelect (result, _fields) => {
    Js.log2 "result" result;

    let id = Js.Json.decodeObject result
        >>= flip Js.Dict.get "insertId"
        >>= Js.Json.decodeNumber
        |> Option.get;

    Mysql.query conn ("SELECT * FROM test WHERE id = " ^ (Js.String.make id));
    /* Mysql.query conn "SELECT * FROM test WHERE i = 48"; */
};

external getDate : Js.Json.t => Js.Date.t = "%identity";

doInsert ()
    |> then_ doSelect
    |> then_ (fun (result, _fields) => {
        Js.log "select";
        Js.log2 "result" result;
        Mysql.end_ conn;
    })
    |> catch (fun exn => {
        Js.log2 "error" exn;
        Mysql.end_ conn;
    });