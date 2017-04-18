addEventListener("message", (event) => {
    postMessage(createExpr(event.data));
});

function createExpr(target) {
    function _createExpr(target, expr, leafCount) {
        const digits = 9;
        if(leafCount == digits) {
            const leaves = expr.leaves();
            for(let i = 0; i < leaves.length; i++) {
                leaves[i].value = i + 1;
            }

            return eval(expr.compile()) == target;
        }

        for(const opcode in expr.ops) {
            const op = expr.ops[opcode];
            const leaves = expr.leaves();
            for(const leaf of leaves) {
                if(leaf.parent != null && leaf.parent.value == "concat" && opcode != "concat") {
                    continue;
                }
                leaf.set(opcode);
                if(_createExpr(target, expr, leafCount + 1)) {
                    return true;
                }
                // try different leaf
                leaf.clear();
            }
            // try different op
        }

        return false;
    }

    const ops = {
        concat: (a, b) => `${a}${b}`,
        add: (a, b) => `(${a})+(${b})`,
        sub: (a, b) => `(${a})-(${b})`,
        mul: (a, b) => `(${a})*(${b})`,
        div: (a, b) => `(${a})/(${b})`,
        pow: (a, b) => `Math.pow(${a},${b})`
    };

    const expr = new ExpressionTree(ops);
    return {
        success: _createExpr(target, expr, 1),
        result: expr.compile()
    };
}

class ExpressionTree {
    constructor(ops) {
        this.root = new ExpressionNode();
        this.ops = ops;
    }

    leaves() {
        const list = [];
        this.root.leaves(list);
        return list;
    }

    isEmpty() {
        return this.root.isLeaf();
    }

    compile() {
        return this.root.compile(this.ops);
    }

    clear() {
        this.root.clear();
    }
}

class ExpressionNode {
    constructor() {
        this.clear();
        this.parent = null;
    }

    leaves(list) {
        if(this.isLeaf()) {
            list.push(this);
        } else {
            this.left.leaves(list);
            this.right.leaves(list);
        }
    }

    isLeaf() {
        return this.left == null && this.right == null;
    }

    compile(ops) {
        if(this.value in ops) {
            return `${ops[this.value](this.left.compile(ops), this.right.compile(ops))}`;
        } else {
            return this.value;
        }
    }

    set(value) {
        this.clear();
        this.value = value;
        this.makeChildren();
    }

    makeChildren() {
        this.left = new ExpressionNode();
        this.right = new ExpressionNode();
        this.left.parent = this;
        this.right.parent = this;
    }

    clear() {
        if(this.left != null) {
            this.left.clear();
            this.left.parent = null;
            this.left = null;
        }
        if(this.right != null) {
            this.right.clear();
            this.right.parent = null;
            this.right = null;
        }
        this.value = "";
    }
}
