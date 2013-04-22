using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

namespace ReRenamerCS
{
    class Program
    {
        static IEnumerable<string> Find(string path, string pattern)
        {
            try
            {
                return
                    Directory.GetFiles(path, "*.*", SearchOption.TopDirectoryOnly)
                    .Select(Path.GetFileName)
                    .Where(f => Regex.IsMatch(f, pattern));
            }
            catch (DirectoryNotFoundException)
            {
                Console.WriteLine("Invalid path '{0}'", path);
                return new string[0];
            }
        }

        static void RenameOne(string path, string fromName, string toName, bool whatif)
        {
            Console.WriteLine("{0} -> {1}", fromName, toName);
            if (whatif) return;

            var fullFromName = Path.Combine(path, fromName);
            var fullToName = Path.Combine(path, toName);

            try
            {
                File.Move(fullFromName, fullToName);
            }
            catch (Exception e)
            {
                Console.WriteLine("Unable to rename '{0}' because of '{1}'", fromName, e.Message);
            }          
        }

        static void Rename(string path, string pattern, string replacement, bool whatif, IEnumerable<string> files)
        {
            foreach (var file in files)
            {
                var newName = Regex.Replace(file, pattern, replacement);
                RenameOne(path, file, newName, whatif);
            }
        }

        static int FindAndRename(string path, string pattern, string replacement, bool whatif)
        {
            if (whatif) Console.WriteLine("Test mode");
            Console.WriteLine("Looking for '{0}' in '{1}' and replacing with '{2}'", pattern, path, replacement);

            var files = Find(path, pattern);
            Rename(path, pattern, replacement, whatif, files);
            
            return 0;
        }

        static int Main(string[] args)
        {
            if (args.Length < 3 || args.Length > 4)
            {
                Console.WriteLine("Usage: ReRenamer <path> <pattern> <replacement> [-whatif]");
                return 1;
            }

            var path = args[0];
            var pattern = args[1];
            var replacement = args[2];
            var whatif = args.Length > 3 && args[3] == "-whatif";

            return FindAndRename(path, pattern, replacement, whatif);
        }
    }
}
