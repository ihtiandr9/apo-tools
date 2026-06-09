namespace Bkasm
{
    public class ASTree
    {
        public NodeList firstNode;
        public NodeList lastNode;

        private void AppendNode(Node statement)
        {
            var nl = new NodeList();
            nl.node = statement;
            if (lastNode != null)
            {
                lastNode.next = nl;
                lastNode = nl;
            }
            else
            {
                firstNode = nl;
                lastNode = nl;
            }
        }

        public void AddStatement(Node statement)
        {
            if (statement == null)
                return;

            switch (statement.type)
            {
                case eNodeType.NODE_INSTRUCTION:
                case eNodeType.NODE_PSEUDO:
                    AppendNode(statement);
                    break;

                case eNodeType.NODE_VAR:
                    switch (statement.label.targetType)
                    {
                        case eIdentType.TOK_REGPC:
                        case eIdentType.TOK_IDENT:
                            AppendNode(statement);
                            AsmVars.Add(statement.ident, statement.label.target.value);
                            break;
                    }
                    break;
            }
        }
    }
}
