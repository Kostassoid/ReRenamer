open System
open System.IO
open System.Text.RegularExpressions

let find path pattern =
    try
        Directory.GetFiles(path, "*.*", SearchOption.TopDirectoryOnly)
        |> Seq.map (Path.GetFileName)
        |> Seq.where (fun n -> Regex.IsMatch (n, pattern))
    with
        | :? DirectoryNotFoundException -> printfn "Invalid path '%s'" path; Seq.empty<string>

let renameOne path fromName toName whatif =
    printfn "%s -> %s" fromName toName
    if not whatif then
        let fullFromName = Path.Combine (path, fromName)
        let fullToName = Path.Combine (path, toName)
        try
            File.Move (fullFromName, fullToName)
        with
            | e -> printfn "Unable to rename '%s' because of '%s'" fromName e.Message

let rename path pattern (replacement:string) whatif files =
    files
    |> Seq.map(fun n -> (n, (Regex.Replace (n, pattern, replacement))))
    |> Seq.iter(fun (f, t) -> renameOne path f t whatif )

let findAndRename path pattern replacement whatif =
    if (whatif) then printfn "Test mode"
    printfn "Looking for '%s' in '%s' and replacing with '%s'" pattern path replacement
    find path pattern
    |> rename path pattern replacement whatif
    0

[<EntryPoint>]
let main argv = 
    match argv with
    | [| path; pattern; replacement |] -> findAndRename path pattern replacement false
    | [| path; pattern; replacement; whatif |] -> findAndRename path pattern replacement (whatif = "-whatif")
    | _ ->
        printfn "Usage: ReRenamer <path> <pattern> <replacement> [-whatif]"
        1
        

