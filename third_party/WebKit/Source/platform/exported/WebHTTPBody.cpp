/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "public/platform/WebHTTPBody.h"

#include "platform/FileMetadata.h"
#include "platform/network/EncodedFormData.h"

namespace blink {

class WebHTTPBodyPrivate : public EncodedFormData {};

void WebHTTPBody::Initialize() {
  Assign(static_cast<WebHTTPBodyPrivate*>(EncodedFormData::Create().LeakRef()));
}

void WebHTTPBody::Reset() {
  Assign(0);
}

void WebHTTPBody::Assign(const WebHTTPBody& other) {
  WebHTTPBodyPrivate* p = const_cast<WebHTTPBodyPrivate*>(other.private_);
  if (p)
    p->Ref();
  Assign(p);
}

size_t WebHTTPBody::ElementCount() const {
  DCHECK(!IsNull());
  return private_->Elements().size();
}

bool WebHTTPBody::ElementAt(size_t index, Element& result) const {
  DCHECK(!IsNull());

  if (index >= private_->Elements().size())
    return false;

  const FormDataElement& element = private_->Elements()[index];

  result.data.Reset();
  result.file_path.Reset();
  result.file_start = 0;
  result.file_length = 0;
  result.modification_time = InvalidFileTime();
  result.blob_uuid.Reset();

  switch (element.type_) {
    case FormDataElement::kData:
      result.type = Element::kTypeData;
      result.data.Assign(element.data_.data(), element.data_.size());
      break;
    case FormDataElement::kEncodedFile:
      result.type = Element::kTypeFile;
      result.file_path = element.filename_;
      result.file_start = element.file_start_;
      result.file_length = element.file_length_;
      result.modification_time = element.expected_file_modification_time_;
      break;
    case FormDataElement::kEncodedBlob:
      result.type = Element::kTypeBlob;
      result.blob_uuid = element.blob_uuid_;
      break;
    case FormDataElement::kEncodedFileSystemURL:
      result.type = Element::kTypeFileSystemURL;
      result.file_system_url = element.file_system_url_;
      result.file_start = element.file_start_;
      result.file_length = element.file_length_;
      result.modification_time = element.expected_file_modification_time_;
      break;
    default:
      NOTREACHED();
      return false;
  }

  return true;
}

void WebHTTPBody::AppendData(const WebData& data) {
  EnsureMutable();
  // FIXME: FormDataElement::m_data should be a SharedBuffer<char>.  Then we
  // could avoid this buffer copy.
  private_->AppendData(data.Data(), data.size());
}

void WebHTTPBody::AppendFile(const WebString& file_path) {
  EnsureMutable();
  private_->AppendFile(file_path);
}

void WebHTTPBody::AppendFileRange(const WebString& file_path,
                                  long long file_start,
                                  long long file_length,
                                  double modification_time) {
  EnsureMutable();
  private_->AppendFileRange(file_path, file_start, file_length,
                            modification_time);
}

void WebHTTPBody::AppendFileSystemURLRange(const WebURL& url,
                                           long long start,
                                           long long length,
                                           double modification_time) {
  // Currently we only support filesystem URL.
  DCHECK(KURL(url).ProtocolIs("filesystem"));
  EnsureMutable();
  private_->AppendFileSystemURLRange(url, start, length, modification_time);
}

void WebHTTPBody::AppendBlob(const WebString& uuid) {
  EnsureMutable();
  private_->AppendBlob(uuid, nullptr);
}

long long WebHTTPBody::Identifier() const {
  DCHECK(!IsNull());
  return private_->Identifier();
}

void WebHTTPBody::SetIdentifier(long long identifier) {
  EnsureMutable();
  return private_->SetIdentifier(identifier);
}

bool WebHTTPBody::ContainsPasswordData() const {
  return private_->ContainsPasswordData();
}

void WebHTTPBody::SetContainsPasswordData(bool contains_password_data) {
  private_->SetContainsPasswordData(contains_password_data);
}

WebHTTPBody::WebHTTPBody(PassRefPtr<EncodedFormData> data)
    : private_(static_cast<WebHTTPBodyPrivate*>(data.LeakRef())) {}

WebHTTPBody& WebHTTPBody::operator=(PassRefPtr<EncodedFormData> data) {
  DCHECK(static_cast<WebHTTPBodyPrivate*>(data.LeakRef()));
  return *this;
}

WebHTTPBody::operator PassRefPtr<EncodedFormData>() const {
  return private_;
}

void WebHTTPBody::Assign(WebHTTPBodyPrivate* p) {
  // p is already ref'd for us by the caller
  if (private_)
    private_->Deref();
  private_ = p;
}

void WebHTTPBody::EnsureMutable() {
  DCHECK(!IsNull());
  if (!private_->HasOneRef())
    Assign(static_cast<WebHTTPBodyPrivate*>(private_->Copy().LeakRef()));
}

}  // namespace blink
