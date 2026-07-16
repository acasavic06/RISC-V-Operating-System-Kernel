.global _ZN7kThread13contextSwitchEPNS_7ContextES1_
.type _ZN7kThread13contextSwitchEPNS_7ContextES1_,@function
_ZN7kThread13contextSwitchEPNS_7ContextES1_:

    beqz a0, load_next

    sd ra, 0 * 8(a0)
    sd sp, 1 * 8 (a0)
load_next:
    ld ra, 0 * 8(a1)
    ld sp, 1 * 8 (a1)

    ret