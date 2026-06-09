using System;
using System.Collections.Generic;

namespace Bkasm
{
    public static class AsmVars
    {
        private static Dictionary<string, int> vars = new Dictionary<string, int>(Constants.MAX_VAR_COUNT);

        public static void Add(string key, int val)
        {
            if (string.IsNullOrEmpty(key))
            {
                Console.Error.WriteLine("INTERNAL_ERROR: asmvars_add: key is null");
                return;
            }

            if (Globals.bkasm_stage == BkasmStage.PARSE_STAGE)
            {
                if (vars.ContainsKey(key))
                {
                    ErrorHandler.ThrowError(eErrorType.E_LINKERERROR, "\nDuplicate label: " + key + "\n");
                    Globals.ExitNicely(eErrorType.E_LINKERERROR);
                    return;
                }
            }

            vars[key] = val;
        }

        public static bool Value(string key, out int result)
        {
            if (string.IsNullOrEmpty(key))
            {
                Console.Error.WriteLine("INTERNAL_ERROR: asmvars_value: key is null");
                result = 0;
                return false;
            }

            if (vars.TryGetValue(key, out result))
                return true;

            if (Globals.bkasm_stage > BkasmStage.EVAL_STAGE)
            {
                ErrorHandler.ThrowError(eErrorType.E_LINKERERROR, "\nUndefined variable " + key + "\n");
            }
            return false;
        }

        public static void Print()
        {
            var sorted = new List<KeyValuePair<string, int>>(vars);
            sorted.Sort((a, b) => string.Compare(a.Key, b.Key, StringComparison.Ordinal));
            foreach (var kv in sorted)
            {
                Console.WriteLine("{0} = {1}", kv.Key, kv.Value);
            }
        }
    }
}
