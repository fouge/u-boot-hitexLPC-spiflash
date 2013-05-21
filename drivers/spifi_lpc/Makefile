# Based on U-Boot from Denx
# Added by Diwel, Rennes, FR
#

include $(TOPDIR)/config.mk

LIB	:= $(obj)libspifi_drv_M4.a

all:	$(LIB)

$(LIB):	$(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
