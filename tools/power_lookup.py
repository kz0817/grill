#!/usr/bin/env python3
import argparse


class Context(object):
    def __init__(self):
        self.order_pow2 = None
        self.powers = {}


def calc_inverse(args, ctx):
    w = 1
    for i in range(ctx.order_pow2-1, -1, -1):
        w = (w * ctx.powers[i]) % args.modulo
        print('exp: -2^%s: %s' % (i, w))


def run(args, ctx):
    print('w: %s, modulo: %s (%x)' %
          (args.primitive_root, args.modulo, args.modulo))

    w = args.primitive_root
    for i in range(0, args.max_calculations):
        print('exp: 2^%s: %s' % (i, w))
        if w == 1:
            ctx.order_pow2 = i
            break
        ctx.powers[i] = w
        w = (w * w) % args.modulo

    if ctx.order_pow2 is not None:
        calc_inverse(args, ctx)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('primitive_root', type=int)
    parser.add_argument('modulo', type=int)
    parser.add_argument('-n', '--max-calculations', metavar='N', type=int,
                        default=100)
    args = parser.parse_args()
    ctx = Context()
    run(args, ctx)


if __name__ == '__main__':
    main()
