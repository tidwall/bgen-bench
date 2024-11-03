extern crate rand;

use rand::Rng;
use std::collections::BTreeSet;
use std::time::Instant;
use num_format::{Locale, ToFormattedString};
use std::io::Write;

const N:usize = 1000000;
const G:usize = 50;
const C:i32 = 0;         // -1 = worse-case, 0 = average, +1 = best-case

fn bench_print(n:usize, elapsed:f64) {
    print!("{:>9} ops in ", n.to_formatted_string(&Locale::en));
    print!("{:7.3} secs ", elapsed);
    print!("{:8.1} ns/op ", elapsed*1e9/(n as f64));
    print!("{:>13} op/sec", 
        (((n as f64)/elapsed) as i64).to_formatted_string(&Locale::en));
    println!();
}

fn reset_tree(db: &mut BTreeSet<i32>, keys: &mut Vec<i32>) {
    db.clear();
    rand::thread_rng().shuffle(keys);
    for i in 0..keys.len() {
        db.insert(keys[i]);
    }
}

fn run_op(label: &str, n:usize, g:usize, c:i32, db: &mut BTreeSet<i32>, 
    keys: &mut Vec<i32>, 
    pre: &dyn Fn(&mut BTreeSet<i32>, &mut Vec<i32>),
    op: &dyn Fn(&mut BTreeSet<i32>, &mut Vec<i32>))
{
    let mut telapsed;
    if c < 1 {
        telapsed = 0.0;
    } else {
        telapsed = 9999.0;
    }
    for i in 0..g {
        print!("\r{:19}{}/{} ", label, i, g);
        std::io::stdout().flush().unwrap();
        pre(db, keys);
        let start = Instant::now();
        op(db, keys);
        let elapsed = start.elapsed().as_secs_f64();
        if c == -1 {
            if elapsed > telapsed {
                telapsed = elapsed;
            }
        } else if c == 0 {
            telapsed += elapsed;
        } else {
            if elapsed < telapsed {
                telapsed = elapsed;
            }
        }
    }
    print!("\r{:19}", label);
    if c == 0 {
        telapsed /= g as f64
    }
    bench_print(n, telapsed);
    reset_tree(db, keys);
}

fn main() {
    let n = std::env::var("N").unwrap_or("".to_string()).parse().unwrap_or(N);
    let g = std::env::var("G").unwrap_or("".to_string()).parse().unwrap_or(G);
    let c = std::env::var("C").unwrap_or("".to_string()).parse().unwrap_or(C);

    println!("Benchmarking {} items, {} times, taking the {} result", 
        n, g, if c == -1 { "worst" } else if c == 0 { "average" } else { "best" });


    let mut db = BTreeSet::new();
    let mut keys = (0..n).map(|x|x as i32).collect::<Vec<i32>>();
    reset_tree(&mut db, &mut keys);

    run_op("insert(seq)", n, g, c, &mut db, &mut keys, 
        &|db, keys|{
            db.clear();
            keys.sort();
        },
        &|db,keys|{
            for i in 0..n {
                db.insert(keys[i]);
            }
        }
    );

    run_op("insert(rand)", n, g, c, &mut db, &mut keys, 
        &|db, keys|{
            db.clear();
            rand::thread_rng().shuffle(keys);
        },
        &|db,keys|{
            for i in 0..n {
                db.insert(keys[i]);
            }
        }
    );

    run_op("get(seq)", n, g, c, &mut db, &mut keys, 
        &|_,keys|{
            keys.sort();
        },
        &|db,keys|{
            for i in 0..n {
                db.get(&keys[i]);
            }
        }
    );

    run_op("get(rand)", n, g, c, &mut db, &mut keys, 
        &|_, keys|{
            rand::thread_rng().shuffle(keys);
        },
        &|db,keys|{
            for i in 0..n {
                db.get(&keys[i]);
            }
        }
    );

    run_op("delete(seq)", n, g, c, &mut db, &mut keys, 
        &|db, keys|{
            reset_tree(db, keys);
            keys.sort();
        },
        &|db,keys|{
            for i in 0..n {
                db.remove(&keys[i]);
            }
        }
    );

    run_op("delete(rand)", n, g, c, &mut db, &mut keys, 
        &|db, keys|{
            reset_tree(db, keys);
            rand::thread_rng().shuffle(keys);
        },
        &|db,keys|{
            for i in 0..n {
                db.remove(&keys[i]);
            }
        }
    );

    run_op("reinsert(rand)", n, g, c, &mut db, &mut keys, 
        &|_, keys|{
            rand::thread_rng().shuffle(keys);
        },
        &|db,keys|{
            for i in 0..n {
                db.insert(keys[i]);
            }
        }
    );



}
