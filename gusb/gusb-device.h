/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2010-2011 Richard Hughes <richard@hughsie.com>
 * Copyright (C) 2011 Hans de Goede <hdegoede@redhat.com>
 *
 * Licensed under the GNU Lesser General Public License Version 2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GUSB_DEVICE_H__
#define __GUSB_DEVICE_H__

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define G_USB_TYPE_DEVICE		(g_usb_device_get_type ())
#define G_USB_DEVICE(o)			(G_TYPE_CHECK_INSTANCE_CAST ((o), G_USB_TYPE_DEVICE, GUsbDevice))
#define G_USB_IS_DEVICE(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), G_USB_TYPE_DEVICE))
#define G_USB_DEVICE_ERROR		(g_usb_device_error_quark ())

typedef struct _GUsbDevicePrivate	GUsbDevicePrivate;
typedef struct _GUsbDevice		GUsbDevice;
typedef struct _GUsbDeviceClass		GUsbDeviceClass;

/**
 * GUsbDeviceDirection:
 *
 * The message direction.
 **/
typedef enum {
	G_USB_DEVICE_DIRECTION_DEVICE_TO_HOST, /* IN */
	G_USB_DEVICE_DIRECTION_HOST_TO_DEVICE  /* OUT */
} GUsbDeviceDirection;

/**
 * GUsbDeviceRequestType:
 *
 * The message request type.
 **/
typedef enum {
	G_USB_DEVICE_REQUEST_TYPE_STANDARD,
	G_USB_DEVICE_REQUEST_TYPE_CLASS,
	G_USB_DEVICE_REQUEST_TYPE_VENDOR,
	G_USB_DEVICE_REQUEST_TYPE_RESERVED
} GUsbDeviceRequestType;

/**
 * GUsbDeviceRecipient:
 *
 * The message recipient.
 **/
typedef enum {
	G_USB_DEVICE_RECIPIENT_DEVICE,
	G_USB_DEVICE_RECIPIENT_INTERFACE,
	G_USB_DEVICE_RECIPIENT_ENDPOINT,
	G_USB_DEVICE_RECIPIENT_OTHER
} GUsbDeviceRecipient;

/**
 * GUsbDeviceError:
 *
 * The error code.
 **/
typedef enum {
	G_USB_DEVICE_ERROR_INTERNAL,
	G_USB_DEVICE_ERROR_TIMED_OUT,
	G_USB_DEVICE_ERROR_NOT_SUPPORTED,
	G_USB_DEVICE_ERROR_NO_DEVICE,
	G_USB_DEVICE_ERROR_NOT_OPEN,
	G_USB_DEVICE_ERROR_ALREADY_OPEN,
	G_USB_DEVICE_ERROR_LAST
} GUsbDeviceError;

struct _GUsbDevice
{
	 GObject			 parent;
	 GUsbDevicePrivate		*priv;
};

struct _GUsbDeviceClass
{
	GObjectClass			 parent_class;
};

GType			 g_usb_device_get_type		(void);
GQuark			 g_usb_device_error_quark	(void);

guint8			 g_usb_device_get_bus		(GUsbDevice	*device);
guint8			 g_usb_device_get_address	(GUsbDevice	*device);
gboolean		 g_usb_device_get_descriptor	(GUsbDevice	*device,
							 GError		**error);
guint16			 g_usb_device_get_vid		(GUsbDevice	*device);
guint16			 g_usb_device_get_pid		(GUsbDevice	*device);

gboolean		 g_usb_device_open		(GUsbDevice	*device,
							 guint		 configuration,
							 guint		 interface,
							 GCancellable	*cancellable,
							 GError		**error);
gboolean		 g_usb_device_close		(GUsbDevice	*device,
							 GError		**error);

/* sync -- TODO: use GCancellable and GUsbSource */
gboolean		 g_usb_device_control_transfer	(GUsbDevice	*device,
							 GUsbDeviceDirection direction,
							 GUsbDeviceRequestType request_type,
							 GUsbDeviceRecipient recipient,
							 guint8		 request,
							 guint16	 value,
							 guint16	 idx,
							 guint8		*data,
							 gsize		 length,
							 gsize		*actual_length,
							 guint		 timeout,
							 GCancellable	*cancellable,
							 GError		**error);

gboolean		 g_usb_device_bulk_transfer	(GUsbDevice	*device,
							 guint8		 endpoint,
							 guint8		*data,
							 gsize		 length,
							 gsize		*actual_length,
							 guint		 timeout,
							 GCancellable	*cancellable,
							 GError		**error);

gboolean		 g_usb_device_interrupt_transfer (GUsbDevice	*device,
							 guint8		 endpoint,
							 guint8		*data,
							 gsize		 length,
							 gsize		*actual_length,
							 guint		 timeout,
							 GCancellable	*cancellable,
							 GError		**error);

/* async */

void		 g_usb_device_control_transfer_async	(GUsbDevice	*device,
							 GUsbDeviceDirection direction,
							 GUsbDeviceRequestType request_type,
							 GUsbDeviceRecipient recipient,
							 guint8		 request,
							 guint16	 value,
							 guint16	 idx,
							 guint8		*data,
							 gsize		 length,
							 guint		 timeout,
							 GCancellable	*cancellable,
							 GAsyncReadyCallback callback,
							 gpointer	 user_data);
gssize		 g_usb_device_control_transfer_finish	(GUsbDevice	*device,
							 GAsyncResult	*res,
							 GError		**error);

void		 g_usb_device_bulk_transfer_async	(GUsbDevice	*device,
							 guint8		 endpoint,
							 guint8		*data,
							 gsize		 length,
							 guint		 timeout,
							 GCancellable	*cancellable,
							 GAsyncReadyCallback callback,
							 gpointer	 user_data);
gssize		 g_usb_device_bulk_transfer_finish	(GUsbDevice	*device,
							 GAsyncResult	*res,
							 GError		**error);

void		 g_usb_device_interrupt_transfer_async	(GUsbDevice	*device,
							 guint8		 endpoint,
							 guint8		*data,
							 gsize		 length,
							 guint		 timeout,
							 GCancellable	*cancellable,
							 GAsyncReadyCallback callback,
							 gpointer	 user_data);
gssize		 g_usb_device_interrupt_transfer_finish	(GUsbDevice	*device,
							 GAsyncResult	*res,
							 GError		**error);

G_END_DECLS

#endif /* __GUSB_DEVICE_H__ */
