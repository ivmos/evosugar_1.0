/*****************************************************************************\
|* Copyright (C) 2009 Irontec SL (*)                                         *|
|* Part of the Evosugar project                                              *|
|* Licensed under the GNU General Public License (*)                         *|
|*                                                                           *|
|* (*) Complete information is provided at license.txt                       *|
|*                                                                           *|
\*****************************************************************************/

#include <glib/gi18n-lib.h>
#include <string.h>
#include <stdio.h>

#include <camel/camel-mime-message.h>
#include <camel/camel-folder.h>
#include <camel/camel-exception.h>
#include <camel/camel-multipart.h>
#include <camel/camel-stream-mem.h>

#include <mail/em-popup.h>
#include <mail/em-utils.h>

#include <e-util/e-error.h>

#include <gtk/gtk.h>
#include <gconf/gconf-client.h>
#include <gtk-2.0/gtk/gtkradiotoolbutton.h>

#include "lib/sugarsoapBinding.nsmap"
#include "scrm.h"
#include "gui/rightarrow.xpm"
#include "gui/leftarrow.xpm"
#include "gui/Accounts.xpm"
#include "gui/Contacts.xpm"
#include "gui/Opportunities.xpm"
#include "gui/Project.xpm"
#include "gui/Leads.xpm"
#include "gui/about.xpm"

#define GCONF_KEY_USERNAME "/apps/evolution/eplugin/evosugar/username"
#define GCONF_KEY_PASSWORD "/apps/evolution/eplugin/evosugar/password"
#define GCONF_KEY_SERVER "/apps/evolution/eplugin/evosugar/server"

#define UI_FILE "/evosugar.ui"

#define SEARCH_CHECK_COL    0
#define SEARCH_PIXBUF_COL   1
#define SEARCH_NAME_COL     2
#define ATT_CHECK_COL   0
#define ATT_FILENAME_COL    1

#define G_LOG_DOMAIN    "com.irontec.evosugar"
#define G_LOG_LEVEL_USER_SHIFT  (8)
#define DEBUG_ON        TRUE
#define BREAKPOINT      g_debug(" %s: %d\n", __FILE__, __LINE__)

typedef struct {
   gchar *username;
   gchar *password;
   gchar *server;
   gchar *id;
} ScrmSession;

typedef struct {
   GtkWidget *mainwin;
   GtkWidget *aboutwin;
   GtkWidget *entry_subject_edit;
   GtkWidget *entry_search;
   GtkTreeView *treeview_search;
   GtkTreeView *treeview_attach;
   GtkWidget *body_text_view;
   GtkBuilder *gtkBuilder;
   GtkWidget *button_search;
   GtkWidget *button_submit;
   GtkWidget *radio_button_from;
   GtkWidget *radio_button_to;
} Gui;


typedef struct {
   GtkWidget *username_entry;
   GtkWidget *password_entry;
   GtkWidget *server_entry;
} GuiConfig;

typedef struct {
   GtkTreeModel *model_search;
   GtkTreeModel *model_attach;
   GtkTextBuffer *body_text_buffer;
} Model;

ScrmSession scrm_session = { 0 };
Gui gui = { 0 };
GuiConfig guiconfig = { 0 };
Model model = { 0 };

gboolean search_from = TRUE;
CamelMimeMessage *msg = NULL;
CamelInternetAddress *from = NULL;
CamelInternetAddress *to = NULL;
struct select_USCOREfields scrm_modules_obj = {0};
struct select_USCOREfields *scrm_modules = &scrm_modules_obj;

gchar *msg_body = NULL;
gchar *from_name = NULL;
gchar *from_addr = NULL;
gchar *to_name = NULL;
gchar *to_addr = NULL;
gchar *subject = NULL;
time_t msg_date;

gboolean started = FALSE;

char *util_md5(char *p_string);
void set_login_username(gchar *p_username);
void set_login_password(gchar *p_password);
void com_irontec_evosugar(void *ep, EMPopupTargetSelect *t);
static void on_server_entry_focus_out(GtkEntry *server_entry, GtkEntry *entry2);
static void on_username_entry_focus_out(GtkEntry *username_entry, GtkEntry *entry2);
static GtkWidget *get_config_widget(void);
GtkWidget *e_plugin_lib_get_configure_widget(EPlugin *epl);

gchar *util_md5(gchar *p_string) {
   GChecksum *gchecksum = g_checksum_new(G_CHECKSUM_MD5);
   g_checksum_update(gchecksum, p_string, strlen(p_string));
   return g_checksum_get_string(gchecksum);
}



gchar *gconf_get_username() {
   if (scrm_session.username == NULL) {
      scrm_session.username = gconf_client_get_string(gconf_client_get_default(), GCONF_KEY_USERNAME, NULL);
   }
   return scrm_session.username;
}

gchar *gconf_get_password() {
   if (scrm_session.password == NULL) {
      scrm_session.password = gconf_client_get_string(gconf_client_get_default(), GCONF_KEY_PASSWORD, NULL);
   }
   return scrm_session.password;

}

gchar *gconf_get_server() {
   if (scrm_session.server == NULL) {
      scrm_session.server = gconf_client_get_string(gconf_client_get_default(), GCONF_KEY_SERVER, NULL);
   }
   return scrm_session.server;
}

void gconf_set_username(gchar *p_username) {
   gconf_client_set_string(gconf_client_get_default(), GCONF_KEY_USERNAME, p_username, NULL);
   g_free(scrm_session.username);
   scrm_session.username = NULL;
}

void gconf_set_password(gchar *p_password) {
   gconf_client_set_string(gconf_client_get_default(), GCONF_KEY_PASSWORD, p_password, NULL);
   g_free(scrm_session.password);
   scrm_session.password = NULL;
}

void gconf_set_server(gchar *p_server) {
   gconf_client_set_string(gconf_client_get_default(), GCONF_KEY_SERVER, p_server, NULL);
   g_free(scrm_session.server);
   scrm_session.server = NULL;
}

void gui_show_message(gchar *title, gchar *message) {
   GtkWidget *dialog, *label, *content_area;
   /* Create the widgets */
   dialog = gtk_dialog_new_with_buttons(title,
         gui.mainwin,
         GTK_DIALOG_MODAL,
         GTK_STOCK_OK,
         GTK_RESPONSE_NONE,
         NULL);
   gtk_dialog_set_has_separator(dialog, FALSE);
   content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
   label = gtk_label_new(message);
   /* Ensure that the dialog box is destroyed when the user responds. */
   g_signal_connect_swapped(dialog,
         "response",
         G_CALLBACK(gtk_widget_destroy),
         dialog);
   /* Add the label, and show everything we've added to the dialog. */
   gtk_container_add(GTK_CONTAINER(content_area), label);
   gtk_widget_show_all(dialog);
}

GtkTreeModel *
gui_create_treemodel_search(const char *p_query) {

   GtkTreeStore *tree_store;
   GdkPixbuf *accounts_pixbuf = gdk_pixbuf_new_from_xpm_data(Accounts);
   GdkPixbuf *contacts_pixbuf = gdk_pixbuf_new_from_xpm_data(Contacts);
   GdkPixbuf *opportunities_pixbuf = gdk_pixbuf_new_from_xpm_data(Opportunities);
   GdkPixbuf *project_pixbuf = gdk_pixbuf_new_from_xpm_data(Project);
   GdkPixbuf *leads_pixbuf = gdk_pixbuf_new_from_xpm_data(Leads);

   GtkTreeIter iter, child;




   BREAKPOINT;
   tree_store = gtk_tree_store_new(5, G_TYPE_BOOLEAN, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);


   BREAKPOINT;
   struct select_USCOREfields scrm_modules_obj;
   struct select_USCOREfields *scrm_modules = &scrm_modules_obj;
   BREAKPOINT;
   scrm_get_default_modules(scrm_modules);
   BREAKPOINT;
   struct ns1__get_USCOREentry_USCORElist_USCOREresult *entry_list = scrm_search_by_module(gconf_get_server(), gconf_get_username(), gconf_get_password(), p_query, scrm_modules);


   if (entry_list != NULL) {
      int i = 0;
      if (entry_list->result_USCOREcount > 0) {
         BREAKPOINT;
         for (i = 0; i < entry_list->entry_USCORElist->__size; i++) {
            g_debug("MODULO i: %d, l: <%s>\n", i, entry_list->entry_USCORElist->__ptr[i]->module_USCOREname); //Nombre del modulo
            char *module_name = entry_list->entry_USCORElist->__ptr[i]->module_USCOREname;
            GdkPixbuf *the_pixbuf;
            unsigned int is_contacts_module = FALSE;

            if (g_strcmp0("Accounts", module_name) == 0) {
               the_pixbuf = accounts_pixbuf;
            } else if (g_strcmp0("Contacts", module_name) == 0) {
               the_pixbuf = contacts_pixbuf;
               is_contacts_module = TRUE;
            } else if (g_strcmp0("Opportunities", module_name) == 0) {
               the_pixbuf = opportunities_pixbuf;
            } else if (g_strcmp0("Project", module_name) == 0) {
               the_pixbuf = project_pixbuf;
            } else if (g_strcmp0("Leads", module_name) == 0) {
               the_pixbuf = leads_pixbuf;
            }

            int j;
            //for (j = 0; j < entry_list->entry_USCORElist->__ptr[i]->name_USCOREvalue_USCORElist->__size; j++) {
            //   g_debug("i: %d, l: <%s>\n", i, entry_list->entry_USCORElist->__ptr[i]->name_USCOREvalue_USCORElist->__ptr[j]->value); //bean-id
            //}
            char *item_id = entry_list->entry_USCORElist->__ptr[i]->name_USCOREvalue_USCORElist->__ptr[0]->value;
            char *item_name1 = entry_list->entry_USCORElist->__ptr[i]->name_USCOREvalue_USCORElist->__ptr[1]->value;
            char *item_name2 = NULL;
            char *item_displayname = item_name1;
            if (entry_list->entry_USCORElist->__ptr[i]->name_USCOREvalue_USCORElist->__size > 2) {
               item_name2 = entry_list->entry_USCORElist->__ptr[i]->name_USCOREvalue_USCORElist->__ptr[2]->value;
               item_displayname = g_strconcat(item_name1, " ", item_name2, NULL);
            }

            gtk_tree_store_append(tree_store, &iter, NULL);
            gtk_tree_store_set(tree_store, &iter, 0, FALSE, 1, the_pixbuf,
                  2, item_displayname, 3, item_id, 4, module_name, -1);
            if (is_contacts_module) {
               BREAKPOINT;
               struct contact_USCOREdetail_USCOREarray *contact_detail_array =
                  scrm_get_contact_relationships(gconf_get_server(),
                        gconf_get_username(),
                        gconf_get_password(),
                        item_id);
               BREAKPOINT;
               if (contact_detail_array != NULL) { // TODO why null?
                  int k = 0;
                  for (k = 1; k < contact_detail_array->__size; k++) { // Saltamos el primero, porque es info redundante
                     //BREAKPOINT;
                     //char *item_name1 =
                     char *id = contact_detail_array->__ptr[k]->id;
                     char *sub_item_displayname = g_strconcat(contact_detail_array->__ptr[k]->name1,
                           contact_detail_array->__ptr[k]->name2, NULL);
                     char *module_name = contact_detail_array->__ptr[k]->type;
                     if (g_strcmp0("Account", module_name) == 0) { // TODO WTF?? Porqué en singular?? ARGGGGG!!
                        the_pixbuf = accounts_pixbuf;
                        module_name = "Accounts"; // TODO dirty fix investigar a ver que pasa
                     } else if (g_strcmp0("Contact", module_name) == 0) {
                        the_pixbuf = contacts_pixbuf;
                        module_name = "Contacts";
                        is_contacts_module = TRUE;
                     } else if (g_strcmp0("Opportunity", module_name) == 0) {
                        the_pixbuf = opportunities_pixbuf;
                        module_name = "Opportunities";
                     } else if (g_strcmp0("Project", module_name) == 0) {
                        the_pixbuf = project_pixbuf;
                     } else if (g_strcmp0("Leads", module_name) == 0) {
                        the_pixbuf = leads_pixbuf;
                     } else if (g_strcmp0("Lead", module_name) == 0) {
                        the_pixbuf = leads_pixbuf;
                        module_name = "Leads";
                     }
                     g_debug("Type : %s %s\n", contact_detail_array->__ptr[k]->type, contact_detail_array->__ptr[k]->association);
                     gtk_tree_store_append(tree_store, &child, &iter);
                     gtk_tree_store_set(tree_store, &child, 0, FALSE, 1, the_pixbuf, 2, sub_item_displayname,
                           3, id, 4, module_name, -1);
                     g_free(sub_item_displayname);
                  }
               }

            }
         }
      }

   } else {
      BREAKPOINT;
      GdkPixbuf *the_pixbuf = accounts_pixbuf;
      BREAKPOINT;
      gtk_tree_store_append(tree_store, &iter, NULL);
      BREAKPOINT;
      gtk_tree_store_set(tree_store, &iter, 0, FALSE, 1, the_pixbuf, 2, _("No results"),
            3, "", 4, "", -1);
      BREAKPOINT;
   }

   return GTK_TREE_MODEL(tree_store);

}

/**
 * clean_name code from mail-to-task plugin (gpl)
 **/
static gchar *
clean_name(const guchar *s) {
   GString *out = g_string_new("");
   guint32 c;
   gchar *r;

   while ((c = camel_utf8_getc((const guchar **) & s))) {
      if (!g_unichar_isprint(c) || (c < 0x7f && strchr(" /'\"`&();|<>$%{}!", c)))
         c = '_';
      g_string_append_u(out, c);
   }

   r = g_strdup(out->str);
   g_string_free(out, TRUE);

   return r;
}


static void
on_search_cell_toggled(GtkCellRendererToggle *renderer,
      gchar *path,
      GtkTreeView *p_treeview) {

   GtkTreeModel *model;
   GtkTreeIter iter;
   gboolean value;
   gchar *item_id;
   gchar *module_name;

   GdkPixbuf *gd;
   gchar *name;

   /* Toggle the cell renderer's current state to the logical not. */
   model = gtk_tree_view_get_model(p_treeview);
   if (gtk_tree_model_get_iter_from_string(model, &iter, path)) {
      gtk_tree_model_get(model, &iter, 0, &value, 1, &gd, 2, &name, 3, &item_id,
            4, &module_name, -1);
      BREAKPOINT;

      gtk_tree_store_set(GTK_TREE_STORE(model), &iter, 0, !value, 1, gd, 2, name,
            3, item_id, 4, module_name, -1);
   }
}

static void
on_attachment_cell_toggled(GtkCellRendererToggle *renderer,
      gchar *path,
      GtkTreeView *p_treeview) { // TODO refactor on_*_cell_toggled into one

   GtkTreeModel *model;
   GtkTreeIter iter;
   gboolean value;
   gchar *name;

   /* Toggle the cell renderer's current state to the logical not. */
   model = gtk_tree_view_get_model(p_treeview);
   if (gtk_tree_model_get_iter_from_string(model, &iter, path)) {
      gtk_tree_model_get(model, &iter, 0, &value, 1, &name, -1);
      BREAKPOINT;

      gtk_tree_store_set(GTK_TREE_STORE(model), &iter, 0, !value, 1, name, -1);
   }
}

static void
add_columns_search(GtkTreeView * p_treeview) {
   GtkCellRenderer *renderer;
   GtkTreeViewColumn *column;

   /*****************************************/
   column = gtk_tree_view_column_new();
   gtk_tree_view_column_set_title(column, " ");

   renderer = gtk_cell_renderer_toggle_new();
   g_object_set(renderer, "activatable", TRUE, NULL);
   g_signal_connect(renderer, "toggled", G_CALLBACK(on_search_cell_toggled), p_treeview);
   gtk_tree_view_column_pack_start(column, renderer, FALSE);
   gtk_tree_view_column_set_attributes(column, renderer,
         "active", SEARCH_CHECK_COL, NULL);

   gtk_tree_view_append_column(p_treeview, column);
   /**************************************************/
   column = gtk_tree_view_column_new();
   gtk_tree_view_column_set_title(column, _("Object"));

   renderer = gtk_cell_renderer_pixbuf_new();
   gtk_tree_view_column_pack_start(column, renderer, FALSE);
   gtk_tree_view_column_set_attributes(column, renderer,
         "pixbuf", SEARCH_PIXBUF_COL, NULL);

   gtk_tree_view_append_column(p_treeview, column);

   /*****************************************/
   column = gtk_tree_view_column_new();
   gtk_tree_view_column_set_title(column, _("Name"));
   renderer = gtk_cell_renderer_text_new();
   gtk_tree_view_column_pack_start(column, renderer, FALSE);
   gtk_tree_view_column_set_attributes(column, renderer,
         "text", SEARCH_NAME_COL, NULL);
   gtk_tree_view_append_column(p_treeview, column);

}

static void
add_columns_attachment(GtkTreeView * p_treeview) {
   GtkCellRenderer *renderer;
   GtkTreeViewColumn *column;

   /*****************************************/
   column = gtk_tree_view_column_new();
   gtk_tree_view_column_set_title(column, " ");

   renderer = gtk_cell_renderer_toggle_new();
   g_object_set(renderer, "activatable", TRUE, NULL);
   g_signal_connect(renderer, "toggled", G_CALLBACK(on_search_cell_toggled), p_treeview);
   gtk_tree_view_column_pack_start(column, renderer, FALSE);
   gtk_tree_view_column_set_attributes(column, renderer,
         "active", ATT_CHECK_COL, NULL);

   gtk_tree_view_append_column(p_treeview, column);
   /**************************************************/

   column = gtk_tree_view_column_new();
   gtk_tree_view_column_set_title(column, _("Name"));
   renderer = gtk_cell_renderer_text_new();
   gtk_tree_view_column_pack_start(column, renderer, FALSE);
   gtk_tree_view_column_set_attributes(column, renderer,
         "text", ATT_FILENAME_COL, NULL);
   gtk_tree_view_append_column(p_treeview, column);

}



static void
on_button_search_clicked(const GtkButton *p_button, GtkEntry *p_search_entry) {
   g_debug("button_search clicked %s", gtk_entry_get_text(p_search_entry));
   BREAKPOINT;

   g_debug("VALE : %s\n", gtk_entry_get_text(p_search_entry));
   model.model_search = gui_create_treemodel_search(gtk_entry_get_text(p_search_entry));
   BREAKPOINT;
   gtk_tree_view_set_model(GTK_TREE_VIEW(gui.treeview_search), model.model_search);
   gtk_tree_view_expand_all(GTK_TREE_VIEW(gui.treeview_search));
}

gboolean
foreach_func_rel_email(GtkTreeModel *model,
      GtkTreePath *path,
      GtkTreeIter *iter,
      gchar *email_id) {

   gchar *item_id;
   gboolean on;
   GdkPixbuf *gd;
   gchar *name;
   gchar *module_name;
   GtkTreeIter child;

   BREAKPOINT;

   gtk_tree_model_get(model, iter, 0, &on, 1, &gd, 2, &name,
         3, &item_id, 4, &module_name,
         -1);
   if (on) {
      g_debug("Linkando mail\n");
      scrm_rel_email(gconf_get_server(), scrm_session.id, email_id, module_name, item_id);
   }


   int max = gtk_tree_model_iter_n_children(model, iter);
   int i = 0;

   for (i = 0; i < max; i++) {

      gtk_tree_model_iter_nth_child(model, &child, iter, i);
      gtk_tree_model_get(model, &child, 0, &on, 1, &gd, 2, &name,
            3, &item_id, 4, &module_name,
            -1);

      if (on) {
         g_debug("Linking to child %s %s \n", module_name, item_id);
         scrm_rel_email(gconf_get_server(), scrm_session.id, email_id, module_name, item_id);
      } else {
         g_debug("Hijo NO activo\n");
      }
   }


   return FALSE;
}

gboolean
foreach_func_rel_attachments(GtkTreeModel *model,
      GtkTreePath *path,
      GtkTreeIter *iter,
      gchar *email_id) {

   gboolean on;
   gchar *name;

   BREAKPOINT;

   gtk_tree_model_get(model, iter, 0, &on, 1, &name, -1);
   if (on) {
      BREAKPOINT;
      struct ns1__note_USCOREattachment note;
      struct name_USCOREvalue_USCORElist list;
      list.__size = 2;
      list.__ptr = malloc((list.__size) * sizeof (struct ns1__name_USCOREvalue));
      list.__ptr[0] = (struct ns1__name_USCOREvalue*) malloc(sizeof (struct ns1__name_USCOREvalue));
      list.__ptr[0]->name = "name";
      list.__ptr[0]->value = name;
      list.__ptr[1] = (struct ns1__name_USCOREvalue*) malloc(sizeof (struct ns1__name_USCOREvalue));
      list.__ptr[1]->name = "description";
      list.__ptr[1]->value = "Imported from evosugar";
      BREAKPOINT;

      g_debug("Linkando mail\n");
      BREAKPOINT;
      char *id = scrm_set_entry(gconf_get_server(), scrm_session.id, &list, "Notes");
      BREAKPOINT;
      GError *gerror;

      BREAKPOINT;
      g_debug("Name : %s\n", name);
      if (name != NULL) { // TODO might be bug because of buggy attachment parsing
         gchar *full_filename = g_build_filename("/tmp", g_strdup_printf("%s-%s", "evosugar", clean_name(name)), NULL);
         BREAKPOINT;
         g_debug("HH");
         g_debug("FULL FILENAME %s\n", full_filename);
         /*
            GMappedFile *gmappedfile = g_mapped_file_new(full_filename,
            FALSE, &gerror);
            */
         BREAKPOINT;
         //g_print("Resultado de la lectura %d \n", gerror.code);
         //gchar *str_file = g_mapped_file_get_contents(gmappedfile);
         gchar *str_file;
         //g_print("BLOB %s \n", str_file);
         BREAKPOINT;
         //gsize *size_file = g_mapped_file_get_length(gmappedfile);
         //g_print("MIDE :... %s\n",size_file);

         gsize size_file;
         gboolean ok = g_file_get_contents(full_filename, &str_file, &size_file, &gerror);
         g_debug("Lectura con exito : %d \n", ok);
         struct ns1__note_USCOREattachment note_attach;
         note_attach.filename = name;
         note_attach.id = id;
         BREAKPOINT;
         note_attach.file = g_base64_encode(str_file, size_file);
         char *response_id = set_note_attachment(gconf_get_server(), scrm_session.id,
               &note_attach);

         //g_debug("note id %s \n", response_id);

         relate_note_to_module(gconf_get_server(), scrm_session.id, response_id, "Emails", email_id);
         unlink(full_filename); // delete file
      }
      return 0;

   }


   return FALSE; /* do not stop walking the store, call us with next row */
}

static void
on_button_AddToSugarCRM_clicked(const GtkButton *p_button) {
   g_print("on_button_AddToSugarCRM_clicked");
   BREAKPOINT;
   //struct soap *soap = soap_new();
   BREAKPOINT;


   BREAKPOINT;
   struct name_USCOREvalue_USCORElist list;
   BREAKPOINT;
   list.__size = 9;
   BREAKPOINT;
   struct ns1__name_USCOREvalue element0;
   list.__ptr = malloc((list.__size) * sizeof (struct ns1__name_USCOREvalue));
   list.__ptr[0] = (struct ns1__name_USCOREvalue*) malloc(sizeof (struct ns1__name_USCOREvalue));
   list.__ptr[0]->name = "name";
   list.__ptr[0]->value = gtk_entry_get_text(GTK_ENTRY(gui.entry_subject_edit));
   list.__ptr[1] = (struct ns1__name_USCOREvalue*) malloc(sizeof (struct ns1__name_USCOREvalue));
   list.__ptr[1]->name = "description";
   GtkTextIter text_start;
   GtkTextIter text_end;

   gtk_text_buffer_get_iter_at_line(model.body_text_buffer, &text_start, 0);

   BREAKPOINT;
   gtk_text_buffer_get_iter_at_line(model.body_text_buffer, &text_end, gtk_text_buffer_get_line_count(model.body_text_buffer));
   BREAKPOINT;
   gchar *cur_body_text = gtk_text_buffer_get_text(model.body_text_buffer, &text_start, &text_end, FALSE);
   list.__ptr[1]->value = cur_body_text;
   g_message("Mensaje a enviar %s\n", cur_body_text);
   g_message("from_addr %s\n", from_addr);
   g_message("to_addr %s\n", to_addr);
   BREAKPOINT;

   BREAKPOINT;
   list.__ptr[2] = (struct ns1__name_USCOREvalue*) malloc(sizeof (struct ns1__name_USCOREvalue));
   list.__ptr[2]->name = "description";
   list.__ptr[2]->value = cur_body_text;
   list.__ptr[3] = (struct ns1__name_USCOREvalue*) malloc(sizeof (struct ns1__name_USCOREvalue));
   list.__ptr[3]->name = "from_addr";
   list.__ptr[3]->value = from_addr;
   list.__ptr[4] = (struct ns1__name_USCOREvalue*) malloc(sizeof (struct ns1__name_USCOREvalue));
   list.__ptr[4]->name = "from_name";
   list.__ptr[4]->value = from_name;
   list.__ptr[5] = (struct ns1__name_USCOREvalue*) malloc(sizeof (struct ns1__name_USCOREvalue));
   list.__ptr[5]->name = "to_addrs";
   list.__ptr[5]->value = to_addr;
   list.__ptr[6] = (struct ns1__name_USCOREvalue*) malloc(sizeof (struct ns1__name_USCOREvalue));
   list.__ptr[6]->name = "status";
   list.__ptr[6]->value = "archived";
   list.__ptr[7] = (struct ns1__name_USCOREvalue*) malloc(sizeof (struct ns1__name_USCOREvalue));
   list.__ptr[7]->name = "type";
   list.__ptr[7]->value = "archived";

   list.__ptr[8] = (struct ns1__name_USCOREvalue*) malloc(sizeof (struct ns1__name_USCOREvalue));
   list.__ptr[8]->name = "date_sent";
   struct tm *ts;
   ts = localtime(&msg_date);
   char buf[80];
   //strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", ts);

   strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ts);
   list.__ptr[8]->value = buf;
   //list.__ptr[0] = &element0;
   BREAKPOINT;

   BREAKPOINT;
   char *id = scrm_set_entry(gconf_get_server(), scrm_session.id, &list, "Emails");


   BREAKPOINT;
   util_attachments_to_files(msg);
   BREAKPOINT;
   gtk_tree_model_foreach(GTK_TREE_MODEL(model.model_search), foreach_func_rel_email, id);
   gtk_tree_model_foreach(GTK_TREE_MODEL(model.model_attach), foreach_func_rel_attachments, id);

   int i = 0;
   for (i=0; i< list.__size; i++) {
      free(list.__ptr[i]);
   }

   gtk_widget_destroy(gui.mainwin);

}

static void
on_radio_group_search_changed(const GtkRadioButton *p_radio_button, gchar *p_search) {
   BREAKPOINT;
   GSList *group = gtk_radio_button_get_group(p_radio_button);
   gtk_entry_set_text(gui.entry_search, p_search);

}

void
on_notebookTabMain_switch_page(GtkNotebook *notebook,
      GtkNotebookPage *page,
      guint page_num,
      gpointer user_data) {
   ///////////////
}

   void
on_buttonClose_clicked (GtkButton *button, gpointer user_data)
{
   gtk_widget_destroy(gui.mainwin);
}

   void
on_buttonCloseAbout_clicked (GtkButton *button, gpointer user_data)
{
   //gtk_widget_destroy(gui.aboutwin);
   gtk_widget_set_visible(gui.aboutwin, FALSE);
}

   void
on_buttonAbout_clicked (GtkButton *button, gpointer user_data)
{
   if (gui.aboutwin == NULL) {
      gui.aboutwin =
         GTK_WIDGET(gtk_builder_get_object
               (gui.gtkBuilder, "dialogAbout"));
      gtk_window_set_position(gui.aboutwin, GTK_WIN_POS_CENTER_ON_PARENT);
      GdkPixbuf *about_pixbuf = gdk_pixbuf_new_from_xpm_data(about_xpm);
      GtkWidget *image = GTK_WIDGET(gtk_builder_get_object
            (gui.gtkBuilder, "imageAbout"));
      gtk_window_set_title(gui.aboutwin, "Evosugar");
      gtk_image_set_from_pixbuf(image,about_pixbuf);
      gtk_widget_show_all(gui.aboutwin);
   } else {
      gtk_widget_set_visible(gui.aboutwin, TRUE);
   }
}


/**
 *
 * It stores the attachment list into the model.
 * */
GtkTreeModel *
util_get_msg_attachments(CamelMimeMessage *message) {
   gint parts, i = 0;
   GtkTreeIter iter;
   CamelDataWrapper *content = NULL;
   GtkTreeStore *tree_store = gtk_tree_store_new(2, G_TYPE_BOOLEAN, G_TYPE_STRING);

   content = camel_medium_get_content_object((CamelMedium *) message);
   if (!content || !CAMEL_IS_MULTIPART(content))
      return;

   parts = camel_multipart_get_number(CAMEL_MULTIPART(content));
   if (parts < 1)
      return;

   for (i = 1; i < parts; i++) {
      const gchar *orig_filename;
      CamelMimePart *mime_part;

      mime_part = camel_multipart_get_part(CAMEL_MULTIPART(content), i);

      orig_filename = camel_mime_part_get_filename(mime_part);
      if (orig_filename == NULL) {
         g_debug("No hay filename, esto es un adjunto distinto.\n");
         orig_filename = camel_mime_part_get_description(mime_part);
      }


      gtk_tree_store_append(tree_store, &iter, NULL);
      gtk_tree_store_set(tree_store, &iter, 0, TRUE,
            1, orig_filename, -1);

      g_print("Orig filename : %s\n", orig_filename);
      if (!orig_filename)
         continue;

      BREAKPOINT;
   }

   return GTK_TREE_MODEL(tree_store);
}



/**
 *
 * It dumps attachments to /tmp
 *
 **/
void
util_attachments_to_files(CamelMimeMessage *message) {
   gint parts, i = 0;

   char *store_dir;
   CamelDataWrapper *content = NULL;

   content = camel_medium_get_content_object((CamelMedium *) message);
   if (!content || !CAMEL_IS_MULTIPART(content))
      return;

   parts = camel_multipart_get_number(CAMEL_MULTIPART(content));
   if (parts < 1)
      return;

   store_dir = "/tmp"; // TODO home

   for (i = 1; i < parts; i++) {
      gchar *filename = NULL;
      gchar *path = NULL;
      gchar *tmp = NULL;
      const gchar *orig_filename = NULL;
      CamelMimePart *mime_part = camel_multipart_get_part(CAMEL_MULTIPART(content), i);

      orig_filename = camel_mime_part_get_filename(mime_part);
      g_debug("Orig filename : %s\n", orig_filename);
      if (orig_filename == NULL) {
         g_print("Esto es un adjunto distinto.");
         orig_filename = camel_mime_part_get_description(mime_part);
      }
      if (!orig_filename)
         continue;

      tmp = clean_name((const guchar *) orig_filename);

      filename = g_strdup_printf("%s-%s", "evosugar", tmp);
      path = g_build_filename(store_dir, filename, NULL);
      g_debug("Attach. to file : %s\n", path);
      BREAKPOINT;
      // TODO should be done in another thread
      if (em_utils_save_part_to_file(NULL, path, mime_part)) {
         gchar *uri;
         BREAKPOINT;
         uri = g_filename_to_uri(path, NULL, NULL);
         BREAKPOINT;
         g_free(uri);
      }
      BREAKPOINT;
      g_free(tmp);
      BREAKPOINT;
      g_free(filename);
      BREAKPOINT;
      g_free(path);
      BREAKPOINT;
   }

   //g_free (store_dir);
}


/**
 *
 * "util_get_msg_body" is based on mail-to-task eplugin's get_description function.
 * which is GPL licensed.
 * */
static gchar *
util_get_msg_body(CamelMimeMessage *message, gchar **text) {
   CamelDataWrapper *content;
   CamelStream *mem;
   CamelContentType *type;
   CamelMimePart *mime_part = CAMEL_MIME_PART(message);

   GSList sl;
   gchar *str, *convert_str = NULL;
   gsize bytes_read, bytes_written;
   gint count = 2;

   content = camel_medium_get_content_object((CamelMedium *) message);

   if (!content)
      return;

   /*
    * Get non-multipart content from multipart message.
    */
   while (CAMEL_IS_MULTIPART(content) && count > 0) {
      mime_part = camel_multipart_get_part(CAMEL_MULTIPART(content), 0);
      content = camel_medium_get_content_object(CAMEL_MEDIUM(mime_part));
      count--;
   }

   if (!mime_part)
      return;

   type = camel_mime_part_get_content_type(mime_part);
   if (!camel_content_type_is(type, "text", "plain"))
      return;

   mem = camel_stream_mem_new();
   camel_data_wrapper_decode_to_stream(content, mem);

   str = g_strndup((const gchar *) ((CamelStreamMem *) mem)->buffer->data, ((CamelStreamMem *) mem)->buffer->len);
   camel_object_unref(mem);

   /* convert to UTF-8 string */
   if (str && content->mime_type->params && content->mime_type->params->value) {
      convert_str = g_convert(str, strlen(str),
            "UTF-8", content->mime_type->params->value,
            &bytes_read, &bytes_written, NULL);
   }

   if (convert_str)
      *text = convert_str;
   else
      *text = str;

   /*g_free (str);
     if (convert_str)
     g_free (convert_str);
     */
}

void
com_irontec_evosugar(void *ep, EMPopupTargetSelect *t) {

   CamelException ex = {0};


   gchar *folder_name =  camel_folder_get_name(t->folder);
   gboolean folder_is_sent = FALSE;
   BREAKPOINT;
   gui.gtkBuilder = gtk_builder_new();
   gtk_builder_set_translation_domain(gui.gtkBuilder, GETTEXT_PACKAGE);
   gtk_builder_add_from_file(gui.gtkBuilder, g_build_filename(PLUGIN_INSTALL_DIR, UI_FILE, NULL), NULL);


   gui.entry_search = GTK_WIDGET(gtk_builder_get_object(gui.gtkBuilder, "entrySearch"));
   gui.button_search = GTK_WIDGET(gtk_builder_get_object(gui.gtkBuilder, "buttonSearch"));
   gui.radio_button_from = GTK_WIDGET(gtk_builder_get_object(gui.gtkBuilder, "radiobuttonFrom"));
   gui.radio_button_to = GTK_WIDGET(gtk_builder_get_object(gui.gtkBuilder, "radiobuttonTo"));
   gui.button_submit = GTK_WIDGET(gtk_builder_get_object(gui.gtkBuilder, "buttonAddToSugarCRM"));

   gui.entry_subject_edit = GTK_WIDGET(gtk_builder_get_object(gui.gtkBuilder, "entryEditSubject"));

   BREAKPOINT;
   g_signal_connect(gui.button_search, "clicked",
         G_CALLBACK(on_button_search_clicked), GTK_ENTRY(gui.entry_search));
   g_signal_connect(gui.button_submit, "clicked",
         G_CALLBACK(on_button_AddToSugarCRM_clicked), NULL);

   scrm_session.id = scrm_login(gconf_get_username(), gconf_get_password(), gconf_get_server());

   if ((g_strcmp0(folder_name, "Sent") == 0) || (g_strcmp0(folder_name, _("Sent")) == 0) ) {
      folder_is_sent = TRUE;
   }

   if (scrm_session.id == NULL || g_strcmp0(scrm_session.id, "-1") == 0) {
      gui_show_message(_("SugarCRM"), _("Unable to connect: check your connection and settings please."));
   } else {


      BREAKPOINT;

      msg = camel_folder_get_message(t->folder, t->uids->pdata[0], &ex);
      util_get_msg_body(msg, &msg_body);

      if (msg == NULL) {
         camel_exception_clear(&ex);
         return;
      }
      BREAKPOINT;

      subject = camel_mime_message_get_subject(msg);
      gtk_entry_set_text(gui.entry_subject_edit, subject);

      gui.body_text_view = gtk_builder_get_object(gui.gtkBuilder, "bodyTextView");
      model.body_text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gui.body_text_view));

      gtk_text_buffer_set_text(model.body_text_buffer, msg_body, -1);

      from = camel_mime_message_get_from(msg);
      to = camel_mime_message_get_recipients(msg, CAMEL_RECIPIENT_TYPE_TO);

      BREAKPOINT;
      msg_date = camel_mime_message_get_date(msg, NULL);
      camel_internet_address_get(from, 0, &from_name, &from_addr);
      camel_internet_address_get(to, 0, &to_name, &to_addr);
      g_signal_connect(gui.radio_button_from, "pressed",
            G_CALLBACK(on_radio_group_search_changed), from_addr);

      gchar *to_search = to_addr;


      g_signal_connect(gui.radio_button_to, "pressed",
            G_CALLBACK(on_radio_group_search_changed), to_search);
      if (folder_is_sent) {
         gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui.radio_button_to),TRUE);
      }

      BREAKPOINT;

      GtkWidget *content_box = GTK_WIDGET(gtk_builder_get_object(gui.gtkBuilder, "vboxContents"));
      GtkWidget *frame_attach = GTK_WIDGET(gtk_builder_get_object(gui.gtkBuilder, "frameAttach"));

      if (folder_is_sent) {
         gtk_entry_set_text(gui.entry_search, to_addr);
      } else {
         gtk_entry_set_text(gui.entry_search, from_addr);
      }

      GtkScrolledWindow *scrolledwindow = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new(NULL, NULL));
      gtk_scrolled_window_set_policy(scrolledwindow,
            GTK_POLICY_AUTOMATIC,
            GTK_POLICY_AUTOMATIC);
      GtkScrolledWindow *scrolledwindow2 = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new(NULL, NULL));
      gtk_scrolled_window_set_policy(scrolledwindow2,
            GTK_POLICY_AUTOMATIC,
            GTK_POLICY_AUTOMATIC);



      BREAKPOINT;
      gtk_container_add(GTK_CONTAINER(content_box), scrolledwindow);
      gtk_container_add(GTK_CONTAINER(frame_attach), scrolledwindow2);
      BREAKPOINT;
      gui.treeview_search = gtk_tree_view_new();
      gui.treeview_attach = gtk_tree_view_new();


      add_columns_search(GTK_TREE_VIEW(gui.treeview_search));
      add_columns_attachment(GTK_TREE_VIEW(gui.treeview_attach));
      model.model_attach = util_get_msg_attachments(msg);


      //gtk_tree_view_set_model (GTK_TREE_VIEW (treeview), model_search);
      BREAKPOINT;
      //g_object_unref (G_OBJECT (model_search));

      gtk_builder_connect_signals(gui.gtkBuilder, NULL);
      gtk_tree_view_set_model(GTK_TREE_VIEW(gui.treeview_attach), model.model_attach);
      gtk_container_add(scrolledwindow, gui.treeview_search);
      gtk_container_add(scrolledwindow2, gui.treeview_attach);

      gui.mainwin =
         GTK_WIDGET(gtk_builder_get_object
               (gui.gtkBuilder, "addToSugarCRMwindow"));


      /*
       * Show the application window
       */
      gtk_widget_show_all(gui.mainwin);

      //trigger
      on_button_search_clicked(NULL, gui.entry_search);
      if (DEBUG_ON) {
         //g_mem_profile ();
      }

   } // end-else comprobar conexion
}

static void on_server_entry_focus_out(GtkEntry *server_entry, GtkEntry *entry2) {
   gchar *val = (gchar *) gtk_entry_get_text(GTK_ENTRY(server_entry));
   gchar *result = val;
   if (g_strrstr(val,"soap.php") == NULL) {    // TODO deshardcodear, SOAP API version dependant
      result = g_strconcat(val,"/soap.php",NULL);
   }
   gconf_set_server(result);


}

static void on_username_entry_focus_out(GtkEntry *username_entry, GtkEntry *entry2) {
   gchar *val = (gchar *) gtk_entry_get_text(GTK_ENTRY(username_entry));
   gconf_set_username(val);

}

static void on_password_entry_focus_out(GtkEntry *password_entry, GtkEntry *entry2) {
   gchar *val = (gchar *) gtk_entry_get_text(GTK_ENTRY(password_entry));
   gconf_set_password(util_md5(val));

}

static GtkWidget *
get_config_widget(void) {
   GtkWidget *vbox;
   GtkWidget *username_label;
   GtkWidget *password_label;
   GtkWidget *server_label;

   guiconfig.username_entry = gtk_entry_new();
   guiconfig.password_entry = gtk_entry_new();
   guiconfig.server_entry = gtk_entry_new();


   username_label = gtk_label_new(_("Username :"));
   password_label = gtk_label_new(_("Password :"));
   server_label = gtk_label_new(_("Server :"));

   gtk_entry_set_text(GTK_ENTRY(guiconfig.username_entry), gconf_get_username());
   gtk_entry_set_text(GTK_ENTRY(guiconfig.password_entry), gconf_get_password());
   gtk_entry_set_text(GTK_ENTRY(guiconfig.server_entry), gconf_get_server());

   gtk_entry_set_visibility(GTK_ENTRY(guiconfig.password_entry), FALSE);
   gunichar ch = (gunichar) '*';
   gtk_entry_set_invisible_char(GTK_ENTRY(guiconfig.password_entry), ch);
   g_signal_connect_after(GTK_ENTRY(guiconfig.username_entry), "focus-out-event",
         G_CALLBACK(on_username_entry_focus_out), NULL); // Se usa "after" porque sino pueden pasar cosas raras con el manejo del cursor

   g_signal_connect_after(GTK_ENTRY(guiconfig.password_entry), "focus-out-event",
         G_CALLBACK(on_password_entry_focus_out), NULL); // Se usa "after" porque sino pueden pasar cosas raras con el manejo del cursor

   g_signal_connect_after(GTK_ENTRY(guiconfig.server_entry), "focus-out-event",
         G_CALLBACK(on_server_entry_focus_out), NULL); // Se usa "after" porque sino pueden pasar cosas raras con el manejo del cursor


   vbox = gtk_vbox_new(FALSE, 6);

   gtk_widget_show(username_label);
   gtk_widget_show(guiconfig.username_entry);
   gtk_widget_show(password_label);
   gtk_widget_show(guiconfig.password_entry);
   gtk_widget_show(server_label);
   gtk_widget_show(guiconfig.server_entry);
   gtk_container_add(vbox, username_label);
   gtk_container_add(vbox, guiconfig.username_entry);
   gtk_container_add(vbox, password_label);
   gtk_container_add(vbox, guiconfig.password_entry);
   gtk_container_add(vbox, server_label);
   gtk_container_add(vbox, guiconfig.server_entry);
   gtk_widget_show(vbox);

   return vbox;
}

GtkWidget *
e_plugin_lib_get_configure_widget(EPlugin *epl) {

   g_debug("CONFIGURATION");
   return get_config_widget();

}

void
null_log_func(const gchar *log_domain, GLogLevelFlags log_flags, const gchar *msg, gpointer user_data) {
}

int
e_plugin_lib_enable(EPluginLib *ep, int enable) {
   if (DEBUG_ON) {
      //g_mem_set_vtable (glib_mem_profiler_table);
   }
   if (enable) {
      bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
      bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
      GLogLevelFlags log_mask =
         G_LOG_LEVEL_ERROR | G_LOG_LEVEL_CRITICAL |
         G_LOG_LEVEL_WARNING | G_LOG_FLAG_FATAL | G_LOG_LEVEL_DEBUG; //0xff

      if (DEBUG_ON) {
         g_log_set_handler(G_LOG_DOMAIN, log_mask, g_log_default_handler, NULL);
         //g_mem_set_vtable (glib_mem_profiler_table);
      } else {
         g_log_set_handler(G_LOG_DOMAIN, log_mask, null_log_func, NULL);
      }
      scrm_get_default_modules(scrm_modules);

      g_message(_("Evosugar plugin 1.0-beta8 enabled\n"));
   } else {
      g_message(_("Evosugar plugin disabled\n"));
   }

   return 0;
}
