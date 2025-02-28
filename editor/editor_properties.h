/*************************************************************************/
/*  editor_properties.h                                                  */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef EDITOR_PROPERTIES_H
#define EDITOR_PROPERTIES_H

#include "editor/editor_inspector.h"

class CheckBox;
class ColorPickerButton;
class CreateDialog;
class EditorFileDialog;
class EditorLocaleDialog;
class EditorResourcePicker;
class EditorSpinSlider;
class PropertySelector;
class SceneTreeDialog;
class TextEdit;
class TextureButton;

class EditorPropertyNil : public EditorProperty {
	GDCLASS(EditorPropertyNil, EditorProperty);
	LineEdit *text = nullptr;

public:
	virtual void update_property() override;
	EditorPropertyNil();
};

class EditorPropertyText : public EditorProperty {
	GDCLASS(EditorPropertyText, EditorProperty);
	LineEdit *text = nullptr;

	bool updating = false;
	bool string_name = false;
	void _text_changed(const String &p_string);
	void _text_submitted(const String &p_string);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	static void _bind_methods();

public:
	void set_string_name(bool p_enabled);
	virtual void update_property() override;
	void set_placeholder(const String &p_string);
	void set_secret(bool p_enabled);
	EditorPropertyText();
};

class EditorPropertyMultilineText : public EditorProperty {
	GDCLASS(EditorPropertyMultilineText, EditorProperty);
	TextEdit *text = nullptr;

	AcceptDialog *big_text_dialog = nullptr;
	TextEdit *big_text = nullptr;
	Button *open_big_text = nullptr;

	void _big_text_changed();
	void _text_changed();
	void _open_big_text();
	bool expression = false;

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual void update_property() override;
	EditorPropertyMultilineText(bool p_expression = false);
};

class EditorPropertyTextEnum : public EditorProperty {
	GDCLASS(EditorPropertyTextEnum, EditorProperty);

	HBoxContainer *default_layout = nullptr;
	HBoxContainer *edit_custom_layout = nullptr;

	OptionButton *option_button = nullptr;
	Button *edit_button = nullptr;

	LineEdit *custom_value_edit = nullptr;
	Button *accept_button = nullptr;
	Button *cancel_button = nullptr;

	Vector<String> options;
	bool string_name = false;
	bool loose_mode = false;

	void _emit_changed_value(String p_string);
	void _option_selected(int p_which);

	void _edit_custom_value();
	void _custom_value_submitted(String p_value);
	void _custom_value_accepted();
	void _custom_value_cancelled();

protected:
	virtual void _set_read_only(bool p_read_only) override;
	static void _bind_methods();
	void _notification(int p_what);

public:
	void setup(const Vector<String> &p_options, bool p_string_name = false, bool p_loose_mode = false);
	virtual void update_property() override;
	EditorPropertyTextEnum();
};

class EditorPropertyPath : public EditorProperty {
	GDCLASS(EditorPropertyPath, EditorProperty);
	Vector<String> extensions;
	bool folder = false;
	bool global = false;
	bool save_mode = false;
	EditorFileDialog *dialog = nullptr;
	LineEdit *path = nullptr;
	Button *path_edit = nullptr;

	void _path_selected(const String &p_path);
	void _path_pressed();
	void _path_focus_exited();
	void _drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);
	bool _can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;

protected:
	virtual void _set_read_only(bool p_read_only) override;
	static void _bind_methods();
	void _notification(int p_what);

public:
	void setup(const Vector<String> &p_extensions, bool p_folder, bool p_global);
	void set_save_mode();
	virtual void update_property() override;
	EditorPropertyPath();
};

class EditorPropertyLocale : public EditorProperty {
	GDCLASS(EditorPropertyLocale, EditorProperty);
	EditorLocaleDialog *dialog = nullptr;
	LineEdit *locale = nullptr;
	Button *locale_edit = nullptr;

	void _locale_selected(const String &p_locale);
	void _locale_pressed();
	void _locale_focus_exited();

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void setup(const String &p_hit_string);
	virtual void update_property() override;
	EditorPropertyLocale();
};

class EditorPropertyClassName : public EditorProperty {
	GDCLASS(EditorPropertyClassName, EditorProperty);

private:
	CreateDialog *dialog = nullptr;
	Button *property = nullptr;
	String selected_type;
	String base_type;
	void _property_selected();
	void _dialog_created();

protected:
	virtual void _set_read_only(bool p_read_only) override;
	static void _bind_methods();

public:
	void setup(const String &p_base_type, const String &p_selected_type);
	virtual void update_property() override;
	EditorPropertyClassName();
};

class EditorPropertyMember : public EditorProperty {
	GDCLASS(EditorPropertyMember, EditorProperty);

public:
	enum Type {
		MEMBER_METHOD_OF_VARIANT_TYPE, ///< a method of a type
		MEMBER_METHOD_OF_BASE_TYPE, ///< a method of a base type
		MEMBER_METHOD_OF_INSTANCE, ///< a method of an instance
		MEMBER_METHOD_OF_SCRIPT, ///< a method of a script & base
		MEMBER_PROPERTY_OF_VARIANT_TYPE, ///< a property of a type
		MEMBER_PROPERTY_OF_BASE_TYPE, ///< a property of a base type
		MEMBER_PROPERTY_OF_INSTANCE, ///< a property of an instance
		MEMBER_PROPERTY_OF_SCRIPT, ///< a property of a script & base
	};

private:
	Type hint;
	PropertySelector *selector = nullptr;
	Button *property = nullptr;
	String hint_text;

	void _property_selected(const String &p_selected);
	void _property_select();

protected:
	virtual void _set_read_only(bool p_read_only) override;
	static void _bind_methods();

public:
	void setup(Type p_hint, const String &p_hint_text);
	virtual void update_property() override;
	EditorPropertyMember();
};

class EditorPropertyCheck : public EditorProperty {
	GDCLASS(EditorPropertyCheck, EditorProperty);
	CheckBox *checkbox = nullptr;

	void _checkbox_pressed();

protected:
	virtual void _set_read_only(bool p_read_only) override;
	static void _bind_methods();

public:
	virtual void update_property() override;
	EditorPropertyCheck();
};

class EditorPropertyEnum : public EditorProperty {
	GDCLASS(EditorPropertyEnum, EditorProperty);
	OptionButton *options = nullptr;

	void _option_selected(int p_which);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	static void _bind_methods();

public:
	void setup(const Vector<String> &p_options);
	virtual void update_property() override;
	void set_option_button_clip(bool p_enable);
	EditorPropertyEnum();
};

class EditorPropertyFlags : public EditorProperty {
	GDCLASS(EditorPropertyFlags, EditorProperty);
	VBoxContainer *vbox = nullptr;
	Vector<CheckBox *> flags;
	Vector<uint32_t> flag_values;

	void _flag_toggled(int p_index);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	static void _bind_methods();

public:
	void setup(const Vector<String> &p_options);
	virtual void update_property() override;
	EditorPropertyFlags();
};

///////////////////// LAYERS /////////////////////////

class EditorPropertyLayersGrid : public Control {
	GDCLASS(EditorPropertyLayersGrid, Control);

private:
	Vector<Rect2> flag_rects;
	Rect2 expand_rect;
	bool expand_hovered = false;
	bool expanded = false;
	int expansion_rows = 0;
	int hovered_index = -1;
	bool read_only = false;
	int renamed_layer_index = -1;
	PopupMenu *layer_rename = nullptr;
	ConfirmationDialog *rename_dialog = nullptr;
	LineEdit *rename_dialog_text = nullptr;

	void _rename_pressed(int p_menu);
	void _rename_operation_confirm();
	Size2 get_grid_size() const;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	uint32_t value = 0;
	int layer_group_size = 0;
	int layer_count = 0;
	Vector<String> names;
	Vector<String> tooltips;

	void set_read_only(bool p_read_only);
	virtual Size2 get_minimum_size() const override;
	virtual String get_tooltip(const Point2 &p_pos) const override;
	void gui_input(const Ref<InputEvent> &p_ev) override;
	void set_flag(uint32_t p_flag);
	EditorPropertyLayersGrid();
};

class EditorPropertyLayers : public EditorProperty {
	GDCLASS(EditorPropertyLayers, EditorProperty);

public:
	enum LayerType {
		LAYER_PHYSICS_2D,
		LAYER_RENDER_2D,
		LAYER_NAVIGATION_2D,
		LAYER_PHYSICS_3D,
		LAYER_RENDER_3D,
		LAYER_NAVIGATION_3D,
	};

private:
	EditorPropertyLayersGrid *grid = nullptr;
	void _grid_changed(uint32_t p_grid);
	String basename;
	LayerType layer_type;
	PopupMenu *layers = nullptr;
	TextureButton *button = nullptr;

	void _button_pressed();
	void _menu_pressed(int p_menu);
	void _refresh_names();

protected:
	void _notification(int p_what);
	virtual void _set_read_only(bool p_read_only) override;
	static void _bind_methods();

public:
	void setup(LayerType p_layer_type);
	void set_layer_name(int p_index, const String &p_name);
	String get_layer_name(int p_index) const;
	virtual void update_property() override;
	EditorPropertyLayers();
};

class EditorPropertyInteger : public EditorProperty {
	GDCLASS(EditorPropertyInteger, EditorProperty);
	EditorSpinSlider *spin = nullptr;
	bool setting = false;
	void _value_changed(int64_t p_val);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	static void _bind_methods();

public:
	virtual void update_property() override;
	void setup(int64_t p_min, int64_t p_max, int64_t p_step, bool p_hide_slider, bool p_allow_greater, bool p_allow_lesser, const String &p_suffix = String());
	EditorPropertyInteger();
};

class EditorPropertyObjectID : public EditorProperty {
	GDCLASS(EditorPropertyObjectID, EditorProperty);
	Button *edit = nullptr;
	String base_type;
	void _edit_pressed();

protected:
	virtual void _set_read_only(bool p_read_only) override;
	static void _bind_methods();

public:
	virtual void update_property() override;
	void setup(const String &p_base_type);
	EditorPropertyObjectID();
};

class EditorPropertySignal : public EditorProperty {
	GDCLASS(EditorPropertySignal, EditorProperty);
	Button *edit = nullptr;
	String base_type;
	void _edit_pressed();

protected:
	static void _bind_methods();

public:
	virtual void update_property() override;
	EditorPropertySignal();
};

class EditorPropertyCallable : public EditorProperty {
	GDCLASS(EditorPropertyCallable, EditorProperty);
	Button *edit = nullptr;
	String base_type;

protected:
	static void _bind_methods();

public:
	virtual void update_property() override;
	EditorPropertyCallable();
};

class EditorPropertyFloat : public EditorProperty {
	GDCLASS(EditorPropertyFloat, EditorProperty);
	EditorSpinSlider *spin = nullptr;
	bool setting = false;
	bool angle_in_radians = false;
	void _value_changed(double p_val);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	static void _bind_methods();

public:
	virtual void update_property() override;
	void setup(double p_min, double p_max, double p_step, bool p_hide_slider, bool p_exp_range, bool p_greater, bool p_lesser, const String &p_suffix = String(), bool p_angle_in_radians = false);
	EditorPropertyFloat();
};

class EditorPropertyEasing : public EditorProperty {
	GDCLASS(EditorPropertyEasing, EditorProperty);
	Control *easing_draw = nullptr;
	PopupMenu *preset = nullptr;
	EditorSpinSlider *spin = nullptr;
	bool setting = false;

	bool dragging = false;
	bool full = false;
	bool flip = false;
	bool positive_only = false;

	enum {
		EASING_ZERO,
		EASING_LINEAR,
		EASING_IN,
		EASING_OUT,
		EASING_IN_OUT,
		EASING_OUT_IN,
		EASING_MAX

	};

	void _drag_easing(const Ref<InputEvent> &p_ev);
	void _draw_easing();
	void _set_preset(int);

	void _setup_spin();
	void _spin_value_changed(double p_value);
	void _spin_focus_exited();

	void _notification(int p_what);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	static void _bind_methods();

public:
	virtual void update_property() override;
	void setup(bool p_positive_only, bool p_flip);
	EditorPropertyEasing();
};

class EditorPropertyVector2 : public EditorProperty {
	GDCLASS(EditorPropertyVector2, EditorProperty);
	EditorSpinSlider *spin[2];
	bool setting = false;
	double ratio_xy = 1.0;
	double ratio_yx = 1.0;
	TextureButton *linked = nullptr;
	void _update_ratio();
	void _value_changed(double p_val, const String &p_name);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);

public:
	virtual void update_property() override;
	void setup(double p_min, double p_max, double p_step, bool p_hide_slider, bool p_link = false, const String &p_suffix = String());
	EditorPropertyVector2(bool p_force_wide = false);
};

class EditorPropertyRect2 : public EditorProperty {
	GDCLASS(EditorPropertyRect2, EditorProperty);
	EditorSpinSlider *spin[4];
	bool setting = false;
	void _value_changed(double p_val, const String &p_name);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual void update_property() override;
	void setup(double p_min, double p_max, double p_step, bool p_hide_slider, const String &p_suffix = String());
	EditorPropertyRect2(bool p_force_wide = false);
};

class EditorPropertyVector3 : public EditorProperty {
	GDCLASS(EditorPropertyVector3, EditorProperty);
	EditorSpinSlider *spin[3];
	bool setting = false;
	bool angle_in_radians = false;
	double ratio_yx = 1.0;
	double ratio_zx = 1.0;
	double ratio_xy = 1.0;
	double ratio_zy = 1.0;
	double ratio_xz = 1.0;
	double ratio_yz = 1.0;
	TextureButton *linked = nullptr;
	void _update_ratio();
	void _value_changed(double p_val, const String &p_name);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual void update_property() override;
	virtual void update_using_vector(Vector3 p_vector);
	virtual Vector3 get_vector();
	void setup(double p_min, double p_max, double p_step, bool p_hide_slider, bool p_link = false, const String &p_suffix = String(), bool p_angle_in_radians = false);
	EditorPropertyVector3(bool p_force_wide = false);
};

class EditorPropertyVector2i : public EditorProperty {
	GDCLASS(EditorPropertyVector2i, EditorProperty);
	EditorSpinSlider *spin[2];
	bool setting = false;
	double ratio_xy = 1.0;
	double ratio_yx = 1.0;
	TextureButton *linked = nullptr;
	void _update_ratio();
	void _value_changed(double p_val, const String &p_name);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);

public:
	virtual void update_property() override;
	void setup(int p_min, int p_max, bool p_link = false, const String &p_suffix = String());
	EditorPropertyVector2i(bool p_force_wide = false);
};

class EditorPropertyRect2i : public EditorProperty {
	GDCLASS(EditorPropertyRect2i, EditorProperty);
	EditorSpinSlider *spin[4];
	bool setting = false;
	void _value_changed(double p_val, const String &p_name);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual void update_property() override;
	void setup(int p_min, int p_max, const String &p_suffix = String());
	EditorPropertyRect2i(bool p_force_wide = false);
};

class EditorPropertyVector3i : public EditorProperty {
	GDCLASS(EditorPropertyVector3i, EditorProperty);
	EditorSpinSlider *spin[3];
	bool setting = false;
	double ratio_yx = 1.0;
	double ratio_zx = 1.0;
	double ratio_xy = 1.0;
	double ratio_zy = 1.0;
	double ratio_xz = 1.0;
	double ratio_yz = 1.0;
	TextureButton *linked = nullptr;
	void _update_ratio();
	void _value_changed(double p_val, const String &p_name);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual void update_property() override;
	void setup(int p_min, int p_max, bool p_link = false, const String &p_suffix = String());
	EditorPropertyVector3i(bool p_force_wide = false);
};

class EditorPropertyPlane : public EditorProperty {
	GDCLASS(EditorPropertyPlane, EditorProperty);
	EditorSpinSlider *spin[4];
	bool setting = false;
	void _value_changed(double p_val, const String &p_name);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual void update_property() override;
	void setup(double p_min, double p_max, double p_step, bool p_hide_slider, const String &p_suffix = String());
	EditorPropertyPlane(bool p_force_wide = false);
};

class EditorPropertyQuaternion : public EditorProperty {
	GDCLASS(EditorPropertyQuaternion, EditorProperty);
	BoxContainer *default_layout = nullptr;
	EditorSpinSlider *spin[4];
	bool setting = false;

	Button *warning = nullptr;
	AcceptDialog *warning_dialog = nullptr;

	Label *euler_label = nullptr;
	VBoxContainer *edit_custom_bc = nullptr;
	EditorSpinSlider *euler[3];
	Button *edit_button = nullptr;

	Vector3 edit_euler;

	void _value_changed(double p_val, const String &p_name);
	void _edit_custom_value();
	void _custom_value_changed(double p_val);
	void _warning_pressed();

	bool is_grabbing_euler();

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual void update_property() override;
	void setup(double p_min, double p_max, double p_step, bool p_hide_slider, const String &p_suffix = String(), bool p_hide_editor = false);
	EditorPropertyQuaternion();
};

class EditorPropertyVector4 : public EditorProperty {
	GDCLASS(EditorPropertyVector4, EditorProperty);
	EditorSpinSlider *spin[4];
	bool setting = false;
	void _value_changed(double p_val, const String &p_name);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual void update_property() override;
	void setup(double p_min, double p_max, double p_step, bool p_hide_slider, const String &p_suffix = String());
	EditorPropertyVector4();
};

class EditorPropertyVector4i : public EditorProperty {
	GDCLASS(EditorPropertyVector4i, EditorProperty);
	EditorSpinSlider *spin[4];
	bool setting = false;
	void _value_changed(double p_val, const String &p_name);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual void update_property() override;
	void setup(double p_min, double p_max, const String &p_suffix = String());
	EditorPropertyVector4i();
};

class EditorPropertyAABB : public EditorProperty {
	GDCLASS(EditorPropertyAABB, EditorProperty);
	EditorSpinSlider *spin[6];
	bool setting = false;
	void _value_changed(double p_val, const String &p_name);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual void update_property() override;
	void setup(double p_min, double p_max, double p_step, bool p_hide_slider, const String &p_suffix = String());
	EditorPropertyAABB();
};

class EditorPropertyTransform2D : public EditorProperty {
	GDCLASS(EditorPropertyTransform2D, EditorProperty);
	EditorSpinSlider *spin[6];
	bool setting = false;
	void _value_changed(double p_val, const String &p_name);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual void update_property() override;
	void setup(double p_min, double p_max, double p_step, bool p_hide_slider, const String &p_suffix = String());
	EditorPropertyTransform2D(bool p_include_origin = true);
};

class EditorPropertyBasis : public EditorProperty {
	GDCLASS(EditorPropertyBasis, EditorProperty);
	EditorSpinSlider *spin[9];
	bool setting = false;
	void _value_changed(double p_val, const String &p_name);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual void update_property() override;
	void setup(double p_min, double p_max, double p_step, bool p_hide_slider, const String &p_suffix = String());
	EditorPropertyBasis();
};

class EditorPropertyTransform3D : public EditorProperty {
	GDCLASS(EditorPropertyTransform3D, EditorProperty);
	EditorSpinSlider *spin[12];
	bool setting = false;
	void _value_changed(double p_val, const String &p_name);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual void update_property() override;
	virtual void update_using_transform(Transform3D p_transform);
	void setup(double p_min, double p_max, double p_step, bool p_hide_slider, const String &p_suffix = String());
	EditorPropertyTransform3D();
};

class EditorPropertyProjection : public EditorProperty {
	GDCLASS(EditorPropertyProjection, EditorProperty);
	EditorSpinSlider *spin[16];
	bool setting = false;
	void _value_changed(double p_val, const String &p_name);

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual void update_property() override;
	virtual void update_using_transform(Projection p_transform);
	void setup(double p_min, double p_max, double p_step, bool p_hide_slider, const String &p_suffix = String());
	EditorPropertyProjection();
};

class EditorPropertyColor : public EditorProperty {
	GDCLASS(EditorPropertyColor, EditorProperty);
	ColorPickerButton *picker = nullptr;
	void _color_changed(const Color &p_color);
	void _popup_closed();
	void _picker_created();
	void _picker_opening();

	Color last_color;

protected:
	virtual void _set_read_only(bool p_read_only) override;
	void _notification(int p_what);

public:
	virtual void update_property() override;
	void setup(bool p_show_alpha);
	EditorPropertyColor();
};

class EditorPropertyNodePath : public EditorProperty {
	GDCLASS(EditorPropertyNodePath, EditorProperty);
	Button *assign = nullptr;
	Button *clear = nullptr;
	SceneTreeDialog *scene_tree = nullptr;
	NodePath base_hint;
	bool use_path_from_scene_root = false;
	bool pointer_mode = false;

	Vector<StringName> valid_types;
	void _node_selected(const NodePath &p_path);
	void _node_assign();
	void _node_clear();

	bool can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;
	void drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);
	bool is_drop_valid(const Dictionary &p_drag_data) const;

	String _get_meta_pointer_property() const;
	virtual Variant _get_cache_value(const StringName &p_prop, bool &r_valid) const override;
	virtual StringName _get_revert_property() const override;

protected:
	virtual void _set_read_only(bool p_read_only) override;
	static void _bind_methods();
	void _notification(int p_what);

public:
	virtual void update_property() override;
	void setup(const NodePath &p_base_hint, Vector<StringName> p_valid_types, bool p_use_path_from_scene_root = true, bool p_pointer_mode = false);
	EditorPropertyNodePath();
};

class EditorPropertyRID : public EditorProperty {
	GDCLASS(EditorPropertyRID, EditorProperty);
	Label *label = nullptr;

public:
	virtual void update_property() override;
	EditorPropertyRID();
};

class EditorPropertyResource : public EditorProperty {
	GDCLASS(EditorPropertyResource, EditorProperty);

	EditorResourcePicker *resource_picker = nullptr;
	SceneTreeDialog *scene_tree = nullptr;

	bool use_sub_inspector = false;
	EditorInspector *sub_inspector = nullptr;
	VBoxContainer *sub_inspector_vbox = nullptr;
	bool updating_theme = false;
	bool opened_editor = false;

	void _resource_selected(const Ref<Resource> &p_resource, bool p_inspect);
	void _resource_changed(const Ref<Resource> &p_resource);

	void _viewport_selected(const NodePath &p_path);

	void _sub_inspector_property_keyed(const String &p_property, const Variant &p_value, bool p_advance);
	void _sub_inspector_resource_selected(const Ref<Resource> &p_resource, const String &p_property);
	void _sub_inspector_object_id_selected(int p_id);

	void _open_editor_pressed();
	void _fold_other_editors(Object *p_self);
	void _update_property_bg();
	void _update_preferred_shader();

protected:
	virtual void _set_read_only(bool p_read_only) override;
	static void _bind_methods();
	void _notification(int p_what);

public:
	virtual void update_property() override;
	void setup(Object *p_object, const String &p_path, const String &p_base_type);

	void collapse_all_folding() override;
	void expand_all_folding() override;
	void expand_revertable() override;

	void set_use_sub_inspector(bool p_enable);

	EditorPropertyResource();
};

///////////////////////////////////////////////////
/// \brief The EditorInspectorDefaultPlugin class
///
class EditorInspectorDefaultPlugin : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorDefaultPlugin, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual bool parse_property(Object *p_object, const Variant::Type p_type, const String &p_path, const PropertyHint p_hint, const String &p_hint_text, const uint32_t p_usage, const bool p_wide = false) override;

	static EditorProperty *get_editor_for_property(Object *p_object, const Variant::Type p_type, const String &p_path, const PropertyHint p_hint, const String &p_hint_text, const uint32_t p_usage, const bool p_wide = false);
};

#endif // EDITOR_PROPERTIES_H
