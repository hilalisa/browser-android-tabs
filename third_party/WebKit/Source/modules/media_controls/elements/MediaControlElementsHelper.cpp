// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "modules/media_controls/elements/MediaControlElementsHelper.h"

#include "core/events/Event.h"
#include "core/layout/LayoutSlider.h"
#include "core/layout/api/LayoutSliderItem.h"

namespace blink {

// static
bool MediaControlElementsHelper::IsUserInteractionEvent(Event* event) {
  const AtomicString& type = event->type();
  return type == EventTypeNames::mousedown || type == EventTypeNames::mouseup ||
         type == EventTypeNames::click || type == EventTypeNames::dblclick ||
         event->IsKeyboardEvent() || event->IsTouchEvent();
}

// static
bool MediaControlElementsHelper::IsUserInteractionEventForSlider(
    Event* event,
    LayoutObject* layout_object) {
  // It is unclear if this can be converted to isUserInteractionEvent(), since
  // mouse* events seem to be eaten during a drag anyway, see
  // https://crbug.com/516416.
  if (IsUserInteractionEvent(event))
    return true;

  // Some events are only captured during a slider drag.
  const LayoutSliderItem& slider =
      LayoutSliderItem(ToLayoutSlider(layout_object));
  // TODO(crbug.com/695459#c1): LayoutSliderItem::inDragMode is incorrectly
  // false for drags that start from the track instead of the thumb.
  // Use SliderThumbElement::m_inDragMode and
  // SliderContainerElement::m_touchStarted instead.
  if (!slider.IsNull() && !slider.InDragMode())
    return false;

  const AtomicString& type = event->type();
  return type == EventTypeNames::mouseover ||
         type == EventTypeNames::mouseout ||
         type == EventTypeNames::mousemove ||
         type == EventTypeNames::pointerover ||
         type == EventTypeNames::pointerout ||
         type == EventTypeNames::pointermove;
}

}  // namespace blink
