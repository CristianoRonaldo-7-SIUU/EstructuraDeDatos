using System;

namespace SnakeGame {
    public struct Point {
        public int X;
        public int Y;
        public Point(int x,int y){ X = x; Y = y; }
    }

    public class SinglyList {
        class Node { public Point P; public Node? Next; public Node(Point p){P=p; Next = null;} }
        Node? head = null;
        Node? tail = null;
        public int Count { get; private set; }
        public void InsertFront(Point p){ var n = new Node(p); n.Next = head; head = n; if(tail==null) tail=n; Count++; }
        public Point RemoveBack(){
            if(head==null) throw new InvalidOperationException();
            if(head==tail){ var v=head.P; head=tail=null; Count=0; return v; }
            var cur = head;
            while(cur!.Next != tail) cur = cur.Next!;
            var val = tail!.P;
            cur.Next = null;
            tail = cur;
            Count--;
            return val;
        }
        public bool Contains(Point p){
            var c = head;
            while(c!=null){ if(c.P.X==p.X && c.P.Y==p.Y) return true; c = c.Next; }
            return false;
        }
        public Point[] ToArray(){
            Point[] a = new Point[Count];
            var c = head; int i = 0;
            while(c!=null){ a[i++] = c.P; c = c.Next; }
            return a;
        }
        public Point Head { get { return head!=null ? head.P : new Point(-1,-1); } }
    }
}

