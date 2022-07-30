INCLUDE = ../include
CFLAGS = -ggdb -DINVEL_L -I$(INCLUDE)
CC = gcc
OBJECTS = actual.o adelanto.o afinidad.o anula_t.o aut_cheq.o band_hor.o cambio.o cambio_m.o \
			cierre.o clientes.o cobros.o comprob.o consulta.o coop.o _cr0.o _cr1.o cr_disc2.o \
			cr_disco.o cr_displ.o cr_pant.o c_ticket.o cus.o d_entry.o descuent.o devol.o\
         devoluci.o envases.o externos.o ext_tick.o facrem.o f_fijo.o imp_comp.o\
         imp_item.o import_t.o inf_dif.o invent.o nc.o netcom2.o netcom.o novios.o\
         pagxcaja.o pendient.o perfiles.o promo.o recup.o redondeo.o remito.o retiros.o\
         rollo.o script.o stand_by.o t_espera.o tkt.o t_pago.o t_reproc.o uti_comp.o\
         uti_val.o valida.o vendedor.o ventas2.o ventas.o d_entry.o cobros_f.o\
         cobros_esp.o pagos_esp.o socios.o zip.o med_rein.o
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@


all: $(OBJECTS)