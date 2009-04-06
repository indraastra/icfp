/*
 * Implements the Universal Machine specified in um-spec.txt
 *
 */

import collection.mutable.HashMap;
import Math._
import java.util.ArrayList;
import java.io._;
import scala.Console;

object UniversalMachine {
    final val N_REGISTERS = 8;
    private val arrays = new HashMap[Int, Array[Int]];
    private val registers = new Array[Int](N_REGISTERS);
    private var finger = 0;

    def load_scroll(scrollname: String) = {
        val scroll = new File(scrollname)
        val input_stream = new DataInputStream(new BufferedInputStream(new FileInputStream(scroll)));
        val num_ops = (scroll.length() / 4).asInstanceOf[Int];
        val operations = new Array[Int](num_ops);
        try {
            var n = 0;
            while (n < num_ops) {
                operations(n) = input_stream.readInt();
                n = n + 1;
            }
        } finally {
            input_stream.close();
            arrays += (0 -> operations);
        }
    }

    def spin() = {
        while (cycle()) {}
    }

    def cycle(): Boolean = {
        val operation = arrays(0)(finger);
        finger = finger + 1;
        val opnum = operation >>> 28;
        if (opnum == 13) {
            val A = (operation & 0x0e000000) >> 25;
            val value = operation & 0x01ffffff;
            registers(A) = value;
        } else {
            val A = (operation >> 6) & 7;
            val B = (operation >> 3) & 7;
            val C =  operation       & 7;
            opnum match {
                case 0 => {
                    if (registers(C) != 0) {
                        registers(A) = registers(B);
                    }
                }
                case 1 => {
                    registers(A) = arrays(registers(B))(registers(C));
                }
                case 2 => {
                    arrays(registers(A))(registers(B)) = registers(C);
                }
                case 3 => {
                    registers(A) = registers(B) + registers(C);
                }
                case 4 => {
                    registers(A) = registers(B) * registers(C);
                }
                case 5 => {
                    var rB:Long = registers(B);
                    rB = (rB << 32) >>> 32;
                    var rC:Long = registers(C);
                    rC = (rC << 32) >>> 32;
                    registers(A) = (rB / rC).asInstanceOf[Int];
                }
                case 6 => {
                    registers(A) = ~(registers(B) & registers(C));
                }
                case 7 => {
                    return false;
                }
                case 8 => {
                    val new_array = new Array[Int](registers(C));
                    val some_id = (Iterator.range(MIN_INT, MAX_INT)).find(
                        array_id => array_id != 0 && !(arrays contains array_id)
                    );
                    some_id match {
                        case Some(id) => {
                            arrays += (id -> new_array);
                            registers(B) = id;
                        }
                        case _ => { return false; }
                    }
                }
                case 9 => {
                    arrays -= registers(C);
                }
                case 10 => {
                    print(registers(C).asInstanceOf[Char]);
                }
                case 11 => {
                    var c: Int = Console.in.read();
                    registers(C) = if (c == 4 || c == 10 || c == -1) MIN_INT
                                   else c;
                }
                case 12 => {
                    if (registers(B) != 0) {
                        arrays(0) = arrays(registers(B)).toArray;
                    }
                    finger = registers(C);
                }
            }
        }
        return true;
    }

    def main(args: Array[String]) = {
        UniversalMachine.load_scroll(args(0));
        UniversalMachine.spin();
        println();
    }
}
