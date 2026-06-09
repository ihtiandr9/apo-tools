namespace Bkasm
{
    public class Instruction
    {
        public eIdentType instrType;
        public int opcode;
        public Expr lparam;
        public Expr rparam;
        public Expr immediate;
    }

    public class LabelInfo
    {
        public eIdentType targetType;
        public Expr target;
    }

    public class Node
    {
        public eNodeType type;
        public string ident;
        public Instruction op;
        public LabelInfo label;

        public Node()
        {
            type = eNodeType.NODE_EMPTY;
        }

        public Node(string ident, eIdentType instrType, int opcode)
        {
            type = eNodeType.NODE_INSTRUCTION;
            op = new Instruction();
            op.instrType = instrType;
            op.opcode = opcode;
            int len = ident.Length;
            if (len > Constants.MAX_LABEL_SIZE)
                len = Constants.MAX_LABEL_SIZE;
            this.ident = ident.Substring(0, len);
        }

        public Node(string ident)
        {
            type = eNodeType.NODE_VAR;
            label = new LabelInfo();
            int len = ident.Length;
            if (len > Constants.MAX_LABEL_SIZE)
                len = Constants.MAX_LABEL_SIZE;
            this.ident = ident.Substring(0, len);
        }
    }

    public class NodeList
    {
        public Node node;
        public NodeList next;
    }


}
